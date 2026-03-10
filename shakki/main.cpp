#include <iostream>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <list>
#include <cctype>

#include "kayttoliittyma.h"
#include "Siirto.h"
#include "asema.h"

using namespace std;

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
    const int SYVYYS = 4;

    Asema asema;
    Kayttoliittyma* ui = Kayttoliittyma::getInstance();
    ui->aseta_asema(&asema);

    int bottiVari = 0;
    while (true)
    {
        cout << "Botti pelaa (V)alkea vai (M)usta? ";
        string s;
        getline(cin, s);
        if (s.empty()) continue;

        char c = (char)tolower((unsigned char)s[0]);
        if (c == 'v') { bottiVari = 0; break; }
        if (c == 'm') { bottiVari = 1; break; }

        cout << "Anna V tai M.\n";
    }

    cout << "Botti: " << (bottiVari == 0 ? "VALKEA" : "MUSTA") << "\n\n";

    while (true)
    {
        ui->piirraLauta();

        cout << "Evaluointi (valkea - musta): " << asema.evaluoi() << "\n";
        cout << "Vuoro: " << (asema.getSiirtovuoro() == 0 ? "VALKEA" : "MUSTA") << "\n";

        if (asema.getSiirtovuoro() == bottiVari)
        {
            cout << "Bot thinking... (depth " << SYVYYS << ")\n";

            MinMaxPaluu paluu = asema.minimax(SYVYYS);
            Siirto botSiirto = paluu._parasSiirto;

            cout << "Bot siirto: " << siirtoToStr(botSiirto, asema) << "\n";
            cout << "Bot evaluointi: " << paluu._evaluointiArvo << "\n\n";

            asema.paivitaAsema(&botSiirto);
            continue;
        }

        Siirto s = ui->annaVastustajanSiirto();

        if (s.getAlkuruutu().getRivi() == -1 && s.getAlkuruutu().getSarake() == -1)
            break;

        if (s.onkoLyhytLinna() || s.onkoPitkälinna())
        {
            asema.paivitaAsema(&s);
            continue;
        }

        Nappula* nappula = asema._lauta[s.getAlkuruutu().getRivi()][s.getAlkuruutu().getSarake()];

        list<Siirto> sallitut;
        asema.annaLaillisetSiirrot(sallitut);

        bool loytyi = false;
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

        if (!loytyi)
        {
            cout << "Virheellinen siirto!\n";
            continue;
        }

        if (nappula != nullptr && (nappula == Asema::vs || nappula == Asema::ms))
        {
            int loppuRivi = s.getLoppuruutu().getRivi();
            if ((nappula == Asema::vs && loppuRivi == 0) || (nappula == Asema::ms && loppuRivi == 7))
            {
                wstring valinta = ui->kysyKorotus(nappula == Asema::vs ? 0 : 1);

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

        asema.paivitaAsema(&s);
    }

    return 0;
}