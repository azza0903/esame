#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <map>

using namespace std;

struct Attivita {
    string descrizione;
    string ora_inizio;
    string ora_fine;
    string data;

    // Costruttore
    Attivita(const string& desc = "", const string& inizio = "",
             const string& fine = "", const string& d = "")
        : descrizione(desc), ora_inizio(inizio), ora_fine(fine), data(d) {}
};

class DiarioAttivita {
private:
    vector<Attivita> attivita;
    string nome_file = "attivita.txt";

    // Funzione per pulire lo schermo
    void pulisciSchermo() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    // Funzione per validare il formato dell'ora (HH:MM)
    bool validaOra(const string& ora) {
        if (ora.length() != 5 || ora[2] != ':') return false;

        string ore_str = ora.substr(0, 2);
        string minuti_str = ora.substr(3, 2);

        try {
            int ore = stoi(ore_str);
            int minuti = stoi(minuti_str);
            return (ore >= 0 && ore <= 23 && minuti >= 0 && minuti <= 59);
        } catch (...) {
            return false;
        }
    }

    // Funzione per validare il formato della data (YYYY-MM-DD)
    bool validaData(const string& data) {
        if (data.length() != 10 || data[4] != '-' || data[7] != '-') return false;

        try {
            string anno_str = data.substr(0, 4);
            string mese_str = data.substr(5, 2);
            string giorno_str = data.substr(8, 2);

            int anno = stoi(anno_str);
            int mese = stoi(mese_str);
            int giorno = stoi(giorno_str);

            return (anno >= 2020 && anno <= 2030 &&
                    mese >= 1 && mese <= 12 &&
                    giorno >= 1 && giorno <= 31);
        } catch (...) {
            return false;
        }
    }

public:
    // Costruttore
    DiarioAttivita() {
        caricaDaFile();
    }

    // Destructor
    ~DiarioAttivita() {
        salvaSuFile();
    }

    void aggiungiAttivita() {
        pulisciSchermo();
        Attivita nuova_attivita;

        cout << "\n╔═══════════════════════════════════════╗" << endl;
        cout << "║         AGGIUNGI NUOVA ATTIVITA       ║" << endl;
        cout << "╚═══════════════════════════════════════╝" << endl;

        // Descrizione
        cout << "\nDescrizione attivita: ";
        cin.ignore();
        getline(cin, nuova_attivita.descrizione);

        if (nuova_attivita.descrizione.empty()) {
            cout << "❌ Errore: La descrizione non puo essere vuota!" << endl;
            cout << "Premi Enter per continuare...";
            cin.get();
            return;
        }

        // Ora inizio
        do {
            cout << "Ora inizio (formato HH:MM, esempio: 09:30): ";
            getline(cin, nuova_attivita.ora_inizio);

            if (!validaOra(nuova_attivita.ora_inizio)) {
                cout << "❌ Formato ora non valido! Usa HH:MM (esempio: 09:30)" << endl;
            }
        } while (!validaOra(nuova_attivita.ora_inizio));

        // Ora fine
        do {
            cout << "Ora fine (formato HH:MM, esempio: 10:30): ";
            getline(cin, nuova_attivita.ora_fine);

            if (!validaOra(nuova_attivita.ora_fine)) {
                cout << "❌ Formato ora non valido! Usa HH:MM (esempio: 10:30)" << endl;
            }
        } while (!validaOra(nuova_attivita.ora_fine));

        // Data
        do {
            cout << "Data (formato YYYY-MM-DD, esempio: 2025-05-23): ";
            getline(cin, nuova_attivita.data);

            if (!validaData(nuova_attivita.data)) {
                cout << "❌ Formato data non valido! Usa YYYY-MM-DD (esempio: 2025-05-23)" << endl;
            }
        } while (!validaData(nuova_attivita.data));

        attivita.push_back(nuova_attivita);
        salvaSuFile();

        cout << "\n✅ Attivita salvata con successo!" << endl;
        cout << "Premi Enter per continuare...";
        cin.get();
    }

    void visualizzaAttivita() {
        pulisciSchermo();
        string data;

        cout << "\n╔═══════════════════════════════════════╗" << endl;
        cout << "║        VISUALIZZA ATTIVITA            ║" << endl;
        cout << "╚═══════════════════════════════════════╝" << endl;

        do {
            cout << "\nInserisci la data (formato YYYY-MM-DD, esempio: 2025-05-23): ";
            cin >> data;

            if (!validaData(data)) {
                cout << "❌ Formato data non valido! Usa YYYY-MM-DD" << endl;
            }
        } while (!validaData(data));

        cout << "\n╔═══════════════════════════════════════════════════════════════╗" << endl;
        cout << "║                  ATTIVITA DEL GIORNO " << data << "                  ║" << endl;
        cout << "╚═══════════════════════════════════════════════════════════════╝" << endl;

        vector<Attivita> attivita_giorno;
        for (const auto& att : attivita) {
            if (att.data == data) {
                attivita_giorno.push_back(att);
            }
        }

        if (attivita_giorno.empty()) {
            cout << "\n📝 Nessuna attivita registrata per questo giorno" << endl;
        } else {
            // Ordina per ora di inizio
            sort(attivita_giorno.begin(), attivita_giorno.end(),
                 [](const Attivita& a, const Attivita& b) {
                     return a.ora_inizio < b.ora_inizio;
                 });

            cout << "\n┌──────────┬──────────┬────────────────────────────────────┐" << endl;
            cout << "│  INIZIO  │   FINE   │            DESCRIZIONE             │" << endl;
            cout << "├──────────┼──────────┼────────────────────────────────────┤" << endl;

            for (const auto& att : attivita_giorno) {
                cout << "│ " << setw(8) << att.ora_inizio
                     << " │ " << setw(8) << att.ora_fine
                     << " │ " << setw(34) << att.descrizione.substr(0, 34) << " │" << endl;
            }

            cout << "└──────────┴──────────┴────────────────────────────────────┘" << endl;
            cout << "\n📊 Totale attivita: " << attivita_giorno.size() << endl;
        }

        cout << "\nPremi Enter per continuare...";
        cin.ignore();
        cin.get();
    }

    void visualizzaTutteAttivita() {
        pulisciSchermo();

        cout << "\n╔═══════════════════════════════════════╗" << endl;
        cout << "║         TUTTE LE ATTIVITA             ║" << endl;
        cout << "╚═══════════════════════════════════════╝" << endl;

        if (attivita.empty()) {
            cout << "\n📝 Nessuna attivita registrata" << endl;
        } else {
            // Raggruppa per data
            map<string, vector<Attivita>> attivita_per_data;
            for (const auto& att : attivita) {
                attivita_per_data[att.data].push_back(att);
            }

            for (auto& [data, lista_att] : attivita_per_data) {
                cout << "\n📅 " << data << ":" << endl;
                cout << "─────────────────────────────────────────────────" << endl;

                // Ordina per ora
                sort(lista_att.begin(), lista_att.end(),
                     [](const Attivita& a, const Attivita& b) {
                         return a.ora_inizio < b.ora_inizio;
                     });

                for (const auto& att : lista_att) {
                    cout << "  🕒 " << att.ora_inizio << " - " << att.ora_fine
                         << ": " << att.descrizione << endl;
                }
            }

            cout << "\n📊 Totale attivita registrate: " << attivita.size() << endl;
        }

        cout << "\nPremi Enter per continuare...";
        cin.ignore();
        cin.get();
    }

    void eliminaAttivita() {
        pulisciSchermo();

        if (attivita.empty()) {
            cout << "\n📝 Nessuna attivita da eliminare" << endl;
            cout << "Premi Enter per continuare...";
            cin.ignore();
            cin.get();
            return;
        }

        cout << "\n╔═══════════════════════════════════════╗" << endl;
        cout << "║         ELIMINA ATTIVITA              ║" << endl;
        cout << "╚═══════════════════════════════════════╝" << endl;

        // Mostra tutte le attività con numeri
        for (size_t i = 0; i < attivita.size(); ++i) {
            cout << i + 1 << ". " << attivita[i].data << " "
                 << attivita[i].ora_inizio << "-" << attivita[i].ora_fine
                 << ": " << attivita[i].descrizione << endl;
        }

        cout << "\nInserisci il numero dell'attivita da eliminare (0 per annullare): ";
        int scelta;
        cin >> scelta;

        if (scelta > 0 && scelta <= static_cast<int>(attivita.size())) {
            attivita.erase(attivita.begin() + scelta - 1);
            salvaSuFile();
            cout << "✅ Attivita eliminata con successo!" << endl;
        } else if (scelta != 0) {
            cout << "❌ Numero non valido!" << endl;
        }

        cout << "Premi Enter per continuare...";
        cin.ignore();
        cin.get();
    }

    void salvaSuFile() {
        ofstream file(nome_file);
        for (const auto& att : attivita) {
            file << att.data << "|" << att.ora_inizio << "|"
                 << att.ora_fine << "|" << att.descrizione << endl;
        }
        file.close();
    }

    void caricaDaFile() {
        ifstream file(nome_file);
        string linea;
        while (getline(file, linea)) {
            stringstream ss(linea);
            string data, ora_inizio, ora_fine, descrizione;

            if (getline(ss, data, '|') &&
                getline(ss, ora_inizio, '|') &&
                getline(ss, ora_fine, '|') &&
                getline(ss, descrizione)) {
                attivita.emplace_back(descrizione, ora_inizio, ora_fine, data);
            }
        }
        file.close();
    }

    void mostraMenu() {
        int scelta;

        do {
            pulisciSchermo();

            cout << "\n╔═══════════════════════════════════════════════════════════╗" << endl;
            cout << "║                                                           ║" << endl;
            cout << "║         📓 DIARIO DELLE ATTIVITA GIORNALIERE 📓          ║" << endl;
            cout << "║                                                           ║" << endl;
            cout << "╠═══════════════════════════════════════════════════════════╣" << endl;
            cout << "║                                                           ║" << endl;
            cout << "║  1. ➕ Aggiungi nuova attivita                           ║" << endl;
            cout << "║  2. 👁️  Visualizza attivita di un giorno                 ║" << endl;
            cout << "║  3. 📋 Visualizza tutte le attivita                      ║" << endl;
            cout << "║  4. 🗑️  Elimina attivita                                 ║" << endl;
            cout << "║  5. 🚪 Esci                                              ║" << endl;
            cout << "║                                                           ║" << endl;
            cout << "╚═══════════════════════════════════════════════════════════╝" << endl;

            cout << "\n🎯 Scegli un'opzione (1-5): ";
            cin >> scelta;

            switch (scelta) {
                case 1:
                    aggiungiAttivita();
                    break;
                case 2:
                    visualizzaAttivita();
                    break;
                case 3:
                    visualizzaTutteAttivita();
                    break;
                case 4:
                    eliminaAttivita();
                    break;
                case 5:
                    cout << "\n👋 Arrivederci! Grazie per aver usato il Diario delle Attivita!" << endl;
                    break;
                default:
                    cout << "\n❌ Scelta non valida! Riprova." << endl;
                    cout << "Premi Enter per continuare...";
                    cin.ignore();
                    cin.get();
                    break;
            }
        } while (scelta != 5);
    }
};

int main() {
    // Configura la console per supportare UTF-8
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    cout << "🚀 Avvio del Diario delle Attivita..." << endl;

    DiarioAttivita diario;
    diario.mostraMenu();

    return 0;
}
