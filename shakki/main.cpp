#include <iostream>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <list>
#include <vector>
#include <cctype>

#include "kayttoliittyma.h"
#include "Siirto.h"
#include "asema.h"

using namespace std;

// Converts one board square into chess text form.
// Example:
// - column 0, row 7  -> "a1"
// - column 4, row 6  -> "e2"
//
// Internal board coordinates:
// - columns go from 0 to 7  -> a to h
// - rows go from 0 to 7     -> top to bottom
//
// Chess coordinates:
// - files are a to h
// - ranks are 1 to 8
//
// Because row 0 is the top of the board, rank must be calculated as 8 - row.

static string ruutuToStr(Ruutu r)
{
    char file = char('a' + r.getSarake());
    int rankNum = 8 - r.getRivi();
    char rank = char('0' + rankNum);

    string out;
    out += file;
    out += rank;
    return out;
}

// This function receives a piece pointer and compares it to the shared static
// piece objects stored in Asema. If the pointer does not match anything known, we return '?' as a fallback.
static char nappulaKirjain(Nappula* n)
{
    if (!n) return '?';

    if (n == Asema::vt || n == Asema::mt) return 'T'; // Torni
    if (n == Asema::vr || n == Asema::mr) return 'R'; // Ratsu
    if (n == Asema::vl || n == Asema::ml) return 'L'; // Lahetti
    if (n == Asema::vd || n == Asema::md) return 'D'; // Daami
    if (n == Asema::vk || n == Asema::mk) return 'K'; // Kuningas
    if (n == Asema::vs || n == Asema::ms) return 'S'; // Sotilas

    return '?';
}

// Converts a Siirto object into text so the bot can print its move.
// For a normal move we must figure out which piece moved.
// We do that by looking at the current board position at the move's starting square.
static string siirtoToStr(Siirto& s, Asema& asema)
{
    if (s.onkoLyhytLinna()) return "O-O";
    if (s.onkoPitkälinna()) return "O-O-O";

    Ruutu a = s.getAlkuruutu();
    Ruutu b = s.getLoppuruutu();

    Nappula* mover = asema._lauta[a.getRivi()][a.getSarake()];
    char kirjain = nappulaKirjain(mover);

    string out;
    out += kirjain;
    out += ruutuToStr(a);
    out += "-";
    out += ruutuToStr(b);
    return out;
}

int main()
{
    // Search depth used by the bot.
    // Bigger depth = better search, but also slower.
    const int SYVYYS = 4;

    // Create the board position object.
    Asema asema;
    // Get the singleton UI object and connect it to the current board.
    // The UI needs a pointer to the position so it can draw the board and ask moves relative to the current state.
    Kayttoliittyma* ui = Kayttoliittyma::getInstance();
    ui->aseta_asema(&asema);

    // Undo history:
    // before every actually played move we push the current position here.
    //
    // Undo then restores the latest saved position.
    // Because both players' moves are stored, the current undo logic can jump back one full ply or one full move depending on whose turn it is.
    vector<Asema> aikaisemmat_asemat;

    // Which side the bot plays:
    // 0 = white, 1 = black
    int bottiVari = 0;
    while (true)
    {
        // Ask user which color the bot should play.
        // We keep asking until the user gives either V or M.
        cout << "Botti pelaa (V)alkea vai (M)usta? ";
        string s;
        getline(cin, s);
        if (s.empty()) continue;

        // Use lowercase version of first character so both uppercase and lowercase inputs work.
        char c = (char)tolower((unsigned char)s[0]);
        if (c == 'v') { bottiVari = 0; break; }
        if (c == 'm') { bottiVari = 1; break; }

        cout << "Anna V tai M.\n";
    }

    cout << "Botti: " << (bottiVari == 0 ? "VALKEA" : "MUSTA") << "\n\n";

    // Main game loop.
    // One iteration handles exactly one turn:
    // - draw board
    // - show evaluation and whose turn it is
    // - let either bot or human make a move
    while (true)
    {
        // Draw the current board position.
        ui->piirraLauta();

        // Print current evaluation.
        // Convention in this project:
        // positive  = white is better
        // negative  = black is better
        cout << "Evaluointi (valkea - musta): " << asema.evaluoi() << "\n";
        // Print whose turn it currently is.
        cout << "Vuoro: " << (asema.getSiirtovuoro() == 0 ? "VALKEA" : "MUSTA") << "\n";

        // ------------------------------------------------------------
		// BOT TURN (*beep* *boop*).
        // ------------------------------------------------------------
        // If the side to move matches the bot's chosen color,
        // let minimax search for the best move and play it automatically.
        if (asema.getSiirtovuoro() == bottiVari)
        {
            cout << "Bot thinking... (depth " << SYVYYS << ")\n";

            // Run minimax from the current position.
            // minimax() itself decides whether to call maxi() or mini() based on whose turn it is.
            MinMaxPaluu paluu = asema.minimax(SYVYYS);
            // Store the best move found by the search.
            Siirto botSiirto = paluu._parasSiirto;

            // Print the move in the console.
            cout << "Bot siirto: " << siirtoToStr(botSiirto, asema) << "\n";
            // Print the evaluation score that the search assigned to the chosen line.
            cout << "Bot evaluointi: " << paluu._evaluointiArvo << "\n\n";

            // Save current position so undo can return here later.
            aikaisemmat_asemat.push_back(asema);
            // Apply the bot's move to the real board.
            asema.paivitaAsema(&botSiirto);
            // Continue directly to the next loop iteration.
            continue;
        }

        // ------------------------------------------------------------
        // HUMAN TURN
        // ------------------------------------------------------------
        // Ask the user for a move through the UI.
        //
        // The UI may also encode special commands into a Siirto:
        // - quit -> start square (-1, -1)
        // - undo -> start square (-2, -2)
        Siirto s = ui->annaVastustajanSiirto();

        // Quit command: stop the game loop completely.
        if (s.getAlkuruutu().getRivi() == -1 && s.getAlkuruutu().getSarake() == -1)
            break;
        // Undo command: restore the latest earlier saved position if one exists.
        if (s.getAlkuruutu().getRivi() == -2 && s.getAlkuruutu().getSarake() == -2)
        {
            if (aikaisemmat_asemat.empty())
            {
                cout << "Ei voi undo: ei historiaa.\n\n";
            }
            else
            {
                // Restore most recent saved position.
                asema = aikaisemmat_asemat.back();
                aikaisemmat_asemat.pop_back();

                // If after restoring it is still the bot's turn, restore one more position so undo takes back both the player's last move and the bot's reply.
                if (!aikaisemmat_asemat.empty() && asema.getSiirtovuoro() == bottiVari)
                {
                    asema = aikaisemmat_asemat.back();
                    aikaisemmat_asemat.pop_back();
                }
                // Make sure UI still points to the restored position object.
                ui->aseta_asema(&asema);
                cout << "Undo tehty.\n\n";
            }
            continue;
        }
        // Castling is not a normal start-square to end-square move in the code, so it is checked separately here.
        if (s.onkoLyhytLinna() || s.onkoPitkälinna())
        {
            // Generate all legal moves for the current side.
            list<Siirto> sallitut;
            asema.annaLaillisetSiirrot(sallitut);

            bool loytyi = false;
            // Check whether the castling move given by the user exists among the legal moves.
            for (Siirto& sallittu : sallitut)
            {
                if ((s.onkoLyhytLinna() && sallittu.onkoLyhytLinna()) ||
                    (s.onkoPitkälinna() && sallittu.onkoPitkälinna()))
                {
                    loytyi = true;
                    break;
                }
            }
            // If castling was not legal in this position, reject it.
            if (!loytyi)
            {
                cout << "Virheellinen siirto!\n\n";
                continue;
            }
            // Save current position for undo.
            aikaisemmat_asemat.push_back(asema);
            // Apply the castling move.
            asema.paivitaAsema(&s);
            cout << "\n";
            continue;
        }

        // ------------------------------------------------------------
        // NORMAL MOVE VALIDATION
        // ------------------------------------------------------------
        // Generate all legal moves from the current position.
        list<Siirto> sallitut;
        asema.annaLaillisetSiirrot(sallitut);

        bool loytyi = false;
        // Compare the user's input move to the generated legal moves. A move is accepted if start square and end square both match.
        for (Siirto& sallittu : sallitut)
        {
            if (sallittu.getAlkuruutu().getRivi() == s.getAlkuruutu().getRivi() &&
                sallittu.getAlkuruutu().getSarake() == s.getAlkuruutu().getSarake() &&
                sallittu.getLoppuruutu().getRivi() == s.getLoppuruutu().getRivi() &&
                sallittu.getLoppuruutu().getSarake() == s.getLoppuruutu().getSarake())
            {
                loytyi = true;
                break;
            }
        }
        // Reject illegal moves.
        if (!loytyi)
        {
            cout << "Virheellinen siirto!\n\n";
            continue;
        }
        // ------------------------------------------------------------
       // PROMOTION HANDLING
       // ------------------------------------------------------------
       // If the moving piece is a pawn and it reaches the last rank, ask the user which piece they want to promote to.
       // The chosen promoted piece pointer is stored into s._miksikorotetaan. paivitaAsema() then uses that pointer when updating the board.
        Nappula* nappula = asema._lauta[s.getAlkuruutu().getRivi()][s.getAlkuruutu().getSarake()];
        if (nappula != nullptr && (nappula == Asema::vs || nappula == Asema::ms))
        {
            int loppuRivi = s.getLoppuruutu().getRivi();
            // White promotes on row 0, black promotes on row 7.
            if ((nappula == Asema::vs && loppuRivi == 0) || (nappula == Asema::ms && loppuRivi == 7))
            {
                // Ask UI which piece the pawn should become.
                wstring valinta = ui->kysyKorotus(nappula == Asema::vs ? 0 : 1);
                // Store selected promoted piece.
                if (nappula == Asema::vs)
                {
                    if (valinta == L"D") s._miksikorotetaan = Asema::vd;
                    else if (valinta == L"T") s._miksikorotetaan = Asema::vt;
                    else if (valinta == L"L") s._miksikorotetaan = Asema::vl;
                    else if (valinta == L"R") s._miksikorotetaan = Asema::vr;
                }
                else
                {
                    if (valinta == L"D") s._miksikorotetaan = Asema::md;
                    else if (valinta == L"T") s._miksikorotetaan = Asema::mt;
                    else if (valinta == L"L") s._miksikorotetaan = Asema::ml;
                    else if (valinta == L"R") s._miksikorotetaan = Asema::mr;
                }
            }
        }
        // Save current position for undo before making the move.
        aikaisemmat_asemat.push_back(asema);
        // Apply the accepted move to the board.
        asema.paivitaAsema(&s);
        cout << "\n";
    }

    return 0;
}