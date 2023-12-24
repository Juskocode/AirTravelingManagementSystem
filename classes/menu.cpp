#include "menu.h"

#include <ranges>

// Foreground colors
#define FG_BLACK "\033[30m"
#define FG_RED "\033[31m"
#define FG_GREEN "\033[32m"
#define FG_YELLOW "\033[33m"
#define FG_BLUE "\033[34m"
#define FG_MAGENTA "\033[35m"
#define FG_CYAN "\033[36m"
#define FG_WHITE "\033[37m"

// Background colors
#define BG_BLACK "\033[40m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BG_BLUE "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN "\033[46m"
#define BG_WHITE "\033[47m"

// Text edition
#define BOLD "\033[1m"

#define RESET_COLOR "\033[0m"// Reset color to default



/**
 * @brief Initializes the utilities\n
 */
Menu::Menu() {
    printf("\n");
    printf(BG_RED"===========================================================" RESET_COLOR);
    cout << "\t\t\n\n" << " Bem-vindo!\n (Pressione [0] sempre que quiser voltar atrás)\n\n";
    utilities = new Utils();
}

/**
 * @brief Initial menu where the user is able to choose what he wants to do: Do Operation, Check Information, See Stats\n\n
 */
void Menu::init() {
    string option;
    while(true) {
        cout << " O que deseja fazer hoje?\n\n"
                " [1] Realizar operação\n [2] Consultar informação\n [3] Ver estatísticas\n [4] Sair\n\n Opção: ";

        cin >> option;

        if (option == "1")
            chooseSource();

        else if (option == "2")
            info();

        else if (option == "3")
            statistics();

        else if (option == "4")
            return;

        else if (option == "0")
            cout << "\n Não é possível voltar mais atrás!\n\n";

        else{
            cout << "\n Input inválido, tente novamente. \n\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

/**
 * @brief Closes the menu
 */
void Menu::end() {
    printf("\n");
    printf(BG_RED"===========================================================" RESET_COLOR);
}

/**
 * @brief Allows the user to select between 3 different types of source(Specific Airport/City/Location) when doing an operation.\n\n
 */
void Menu::chooseSource() {
    string option;

    while(true){

        cout << "\n Pretende partir de: \n\n "
                "[1] Um aeroporto em específico\n [2] Uma cidade\n [3] Uma localização\n\n Opção: ";
        cin >> option;

        if (option == "1"){
            string airport = validateAirport();
            if (airport == "0") continue;

            src.clear();
            src.push_back(airport);

            chooseTarget();
            return;
        }

        if (option == "2"){
            string country = validateCountry();
            if (country == "0") continue;

            string city = validateCity(country);
            if (city == "0") continue;

            auto airportsPerCity = utilities->getCity();
            src = airportsPerCity[{country,city}];

            chooseTarget();
            return;
        }

        if (option == "3"){
            src = validateLocal();
            cout << '\n' << "Por volta da area especificada existe(m) o(s) seguinte(s) aeroporto(s): " << "\n";
            for(const auto & s : src){
                printf(BOLD BG_CYAN" %s " RESET_COLOR, s.c_str());
                cout << '\n';
            }
            chooseTarget();
            return;
        }

        if (option == "0"){
            cout << "\n";
            return;
        }

        cout << "\n Input inválido, tente novamente. \n";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }
}

/**
 * @brief Allows the user to choose between 3 different types of targets(Specific Airport/City/Location) during an operation.\n\n
 */
void Menu::chooseTarget() {

    string option;

    while(true){
        cout << "\n Pretende chegar a: \n\n "
                "[1] Um aeroporto em específico\n [2] Uma cidade\n [3] Uma localização\n\n Opção: ";
        cin >> option;
        if (option == "1"){
            string destination = validateAirport();
            if (destination == "0") continue;
            while (src.size() == 1 && destination == src.front()) {
                cout << " Não pode partir e chegar ao mesmo aeroporto num voo :(\n";
                destination = validateAirport();
            }
            dest.clear();
            dest.push_back(destination);

            chooseAirlines(true);
            return;
        }

        if (option == "2"){
            string country = validateCountry();
            if (country == "0") continue;

            string city = validateCity(country);
            if (city == "0") continue;

            auto airportsPerCity = utilities->getCity();
            dest = airportsPerCity[{country,city}];

            chooseAirlines(true);
            return;
        }

        if (option == "3"){
            dest = validateLocal();
            cout << '\n' << "Por volta da area especificada existe(m) o(s) seguinte(s) aeroporto(s): " << "\n";
            for(const auto & s : dest){
                printf(BOLD BG_CYAN" %s " RESET_COLOR, s.c_str());
                cout << '\n';
            }
            chooseAirlines(true);
            return;
        }

        if (option == "0"){
            chooseSource();
            return;
        }

        cout << "\n Input inválido, tente novamente. \n";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
    }
}

/**
 * @brief Asks the user if he wants to select an airline or a set of airlines.\n\n
 * @param op - boolean value to distinguish types of operations.
 */
void Menu::chooseAirlines(bool op) {

    string choice = validateOption("\n Pretende escolher as companhias aéreas a usar? \n\n"
                                   " [1] Sim\n [2] Não\n\n Opção: ");
    if (choice == "0"){
        chooseTarget();
        return;
    }

    if (choice == "1") {
        string airline = validateAirline();
        if (airline == "0") { chooseTarget(); return; }
        airlines.insert(Airline(airline));

        string option = validateOption("\n Deseja inserir mais alguma companhia aérea? \n\n"
                                       " [1] Sim\n [2] Não\n\n Opção: ");
        while (option == "1") {
            airline = validateAirline();
            if (airline == "0") { chooseTarget(); return; }
            airlines.insert(Airline(airline));
            option = validateOption("\n Deseja inserir mais alguma companhia aérea? \n\n"
                                    " [1] Sim\n [2] Não\n\n Opção: ");
        }
    }
    if (op) processOperation();
}

/**
 * @brief After choosing a source, target and airline(or not) it asks the user to choose
 * @brief between two criteria(Minimum number of flights/traveled distance).
 * @brief Where the result might be different.\n\n
 */
void Menu::processOperation() {
    Graph graph = utilities->getGraph();
    auto map = utilities->getMap();
    string option = validateOption("\n Indique o critério a usar: \n\n"
                                   " [1] Número mínimo de voos\n [2] Distância mínima percorrida\n\n Opção: ");
    if (option == "0") {
        chooseAirlines(true);
        return;
    }

    if (option == "1"){
        printf(BOLD FG_GREEN"\n===============================================================\n" RESET_COLOR);
        int nrPath = 0, nrFlights;
        auto flightPath = utilities->processFlight(nrFlights,src,dest,airlines);
        if (nrFlights == INT_MAX) cout << " Não existem voos \n\n";
        else{
            for (const auto& pair : flightPath) {
                string source = pair.first;
                string target = pair.second;
                utilities->getGraph().printPathsByFlights(nrPath,map[source], map[target],airlines);
            }
            if (nrPath != 1) cout << " No total, existem " << nrPath << " trajetos possíveis\n\n";
            else cout << " Apenas existe 1 trajeto possível\n\n";
            cout << " O número mínimo de voos é " << nrFlights << "\n\n";}
    }
    else{
        printf(BOLD FG_GREEN"\n===============================================================\n" RESET_COLOR);
        int nrPath = 0;
        double distance;
        auto flightPath = utilities->processDistance(distance,src,dest,airlines);

        for (const auto& pair : flightPath) {
            string source = pair.first;
            string target = pair.second;
            utilities->getGraph().printPathsByDistance(nrPath,map[source], map[target],airlines);
        }

        if (nrPath != 0) cout << " A distância mínima é " << distance << " km\n\n";
    }

    printf(BOLD FG_CYAN"===============================================================\n" RESET_COLOR);
    airlines.clear();
}

/**
 * @brief Information Menu where the user can obtain certain informations about
 * @brief airport/s airlines, countries, articulation points and diameter.\n\n
 */
void Menu::info(){
    string option;
    while(true){
        cout << "\n Que tipo de informação deseja ver? \n\n "
                "[1] Aeroporto específico\n [2] Aeroportos\n [3] Companhias Aéreas\n [4] Países\n [5] Pontos de articulação\n [6] Diâmetro da rede\n\n Opção: ";
        cin >> option;
        if (option == "1")
            showAirport();

        else if (option == "2")
            showAirports();

        else if (option == "3")
            showAirlines();

        else if (option == "4")
            showCountries();

        else if (option == "5"){
            chooseAirlines(false);
            auto res = utilities->getGraph().articulationPoints(airlines);
            cout << '\n';
            for(auto index: res){
                auto airport=utilities->getGraph().getVertexSet()[index]->getAirport();
                printf(BOLD FG_GREEN" %s" RESET_COLOR, airport.getCode().c_str());
                cout << " : " << airport.getName() << endl;
            }
            airlines.clear();
        }
        else if (option == "6"){
            cout << "\n Diâmetro da rede: ";
            printf(BOLD FG_CYAN"%.0f\n" RESET_COLOR, utilities->getGraph().diameter());
        }

        else if (option == "0") {
            cout << "\n";
            return;
        }
        else{
            cout << "\n Input inválido, tente novamente.\n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

/**
 * @brief After selecting "um aeroporto específico" in Info Menu, the user will be taken to a menu with a
 * @brief vastly amount of options to choose from to get information
 * @brief about a single airport.(Existent Flights/Existent Airlines/Reachable Airports/Cities/Countries in 1 or Y flights)\n\n
 */
void Menu::showAirport(){
    string option;

    while(true){
        cout << "\n A partir de um aeroporto, pretende ver:\n\n "
                "[1] Voos existentes\n [2] Companhias aéreas existentes\n [3] Aeroportos alcançáveis\n [4] Cidades alcançáveis\n [5] Países alcançáveis\n [6] Aeroportos/cidades/países possíveis de alcançar com Y voos\n\n Opção: ";
        cin >> option;
        string airport;
        int source;
        if (option == "1"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = utilities->getMap()[airport];
            cout << "\n";
            auto edges = utilities->getGraph().getVertexSet()[source]->getAdj();
            for (const auto& i: edges){
                string target = utilities->getGraph().getVertexSet()[i.getDest()->getId()]->getAirport().getCode();
                cout << " " << airport << " ---( "<< i.getAirline().getCode() << " )--- " <<  target << endl;
            }
        }
        else if (option == "2"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = utilities->getMap()[airport];
            cout << "\n";
            for (const auto& i: utilities->getGraph().airlinesFromAirport(source)){
                printf(BOLD FG_MAGENTA" -" RESET_COLOR);
                cout << " " << i << endl;
            }
        }
        else if (option == "3"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = utilities->getMap()[airport];
            cout << "\n Aeroportos distintos alcancáveis a partir de " << airport << ":\n\n";
            auto airports=utilities->getGraph().airportsFromAirport(source);
            for(const auto& a: airports){
                printf(BOLD FG_CYAN" %s" RESET_COLOR, a.first.c_str());
                cout << ": " << a.second << endl;
            }
        }
        else if (option == "4"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = utilities->getMap()[airport];
            cout << "\n";
            for (const auto& i: utilities->getGraph().targetsFromAirport(source)){
                printf(BOLD FG_CYAN" %s " RESET_COLOR, i.second.c_str()) ;
                cout <<  "- "<< i.first << endl;
            }
        }
        else if (option == "5"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = utilities->getMap()[airport];
            cout << "\n";
            for (const auto& i: utilities->getGraph().countriesFromAirport(source)){
                printf(BOLD FG_GREEN" -" RESET_COLOR);
                cout << " " << i << endl;
            }
        }
        else if (option == "6"){
            airport = validateAirport();
            if (airport == "0") continue;
            showOptions(airport);
        }

        else if (option == "0")
            return;

        else{
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

/**
 * @brief Gives the user the option to choose a max limit of flights and then gives him the possibility to
 * @brief choose between 3 types of targets(Airports,Cities,Countries).\n\n
 * @brief @param airport - source airport
 */
void Menu::showOptions(const string& airport) {
    int maxFlight = customTop(" Que número máximo de voos pretende realizar: ", 63832);
    string option;
    while(true){
        cout << "\n O que pretende ver?\n\n"
                " [1] Aeroportos\n [2] Cidades\n [3] Países\n\n Opção: ";
        cin >> option;

        if (option == "1") {
            Airport::AirportH airports = utilities->getGraph().listReachableEntities<Airport::AirportH>
                                                                 (utilities->getMap()[airport],  maxFlight);
            cout << "\n A partir de " << airport << " é possível alcançar o(s) seguinte(s) aeroporto(s)" << "\n\n";
            for(const auto& airport_ : airports){
                printf(BOLD FG_GREEN" %s " RESET_COLOR, airport_.getCode().c_str());
                cout << "- " << airport_.getName() << '\n';
            }
            cout << "Nº de aeroportos alcançáveis ";
            printf(BOLD FG_CYAN" %lu \n" RESET_COLOR, airports.size());
        }
        else if (option == "2") {
            Airport::CityH2 cities = utilities->getGraph().listReachableEntities<Airport::CityH2>
                                                          (utilities->getMap()[airport], maxFlight);

            cout << "\n A partir de " << airport << " é possível alcançar a(s) seguinte(s) cidades(s)" << "\n\n";
            for(const auto& city : cities){
                printf(BOLD FG_GREEN" %s " RESET_COLOR, city.second.c_str());
                cout << "- "<< city.first << '\n';
            }
            cout << "Nº de cidades alcançáveis ";
            printf(BOLD FG_CYAN" %lu \n" RESET_COLOR, cities.size());
        }
        else if (option == "3"){
            std::set<std::string> countries = utilities->getGraph().listReachableEntities<std::set<std::string>>
                                                                    (utilities->getMap()[airport], maxFlight);

            cout << "\n A partir de " << airport << " é possível alcançar o(s) seguinte(s) países(s)" << "\n\n";
            for(const auto& country : countries){
                printf(BOLD FG_GREEN" - " RESET_COLOR);
                cout << " " << country << '\n';
            }
            cout << "Nº de paises alcançáveis ";
            printf(BOLD FG_CYAN" %lu \n" RESET_COLOR, countries.size());
        }
        else if (option == "0")
            return;

        else {
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

/**
 * @brief The user can obtain information about all the airports as a collective.
 * @brief List of all airports int the world/country. With the most departures or with most airlines\n\n
 */
void Menu::showAirports(){
    string option;
    while(true){
        cout << "\n Pretende ver os aeroportos:\n\n "
                "[1] Totais\n [2] De um país\n [3] Com mais voos\n [4] Com mais companhias aéreas\n\n Opção: ";
        cin >> option;
        if (option == "1")
            for (const auto& i: utilities->getAirports()) {
                printf(BOLD FG_GREEN" %s" RESET_COLOR, i.getCode().c_str());
                cout << " - " << i.getName() << endl;
            }

        else if (option == "2"){
            string country = validateCountry();
            if (country == "0") continue;
            cout << "\n";
            for (const auto& i : utilities->getAirports())
                if (i.getCountry() == country) {
                    printf(BOLD FG_MAGENTA" %s" RESET_COLOR, i.getCode().c_str());
                    cout << " - " << i.getName() << endl;
                }
        }
        else if (option == "3"){
            Graph graph = utilities->getGraph();
            int choice = showTop(), top;
            if (choice == 1) top = 10;
            else if (choice == 2) top = 20;
            else if (choice == 3) top = customTop("\n Selecione um valor para o top: ", 3020);
            else continue;
            int j = 1;
            for (int i = 0; i < top; i++){
                printf(BOLD FG_CYAN"\n %i" RESET_COLOR, j);
                cout << ". " << graph.flightsPerAirport()[i].second
                     << " - " << graph.flightsPerAirport()[i].first << " voos\n";
                j++;
            }
        }
        else if (option == "4"){
            Graph graph = utilities->getGraph();
            int choice = showTop(), top;
            if (choice == 1) top = 10;
            else if (choice == 2) top = 20;
            else if (choice == 3) top = customTop("\n Selecione um valor para o top: ",3020);
            else continue;
            int j = 1;
            for (int i = 0; i < top; i++){
                printf(BOLD FG_GREEN"\n %i" RESET_COLOR, j);
                cout<< ". " << graph.airlinesPerAirport()[i].second
                    << " - " << graph.airlinesPerAirport()[i].first << " companhias aéreas\n";
                j++;
            }
        }
        else if (option == "0")
            return;
        else {
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

/**
 * @brief The user can obtain information about all the airlines as a collective.
 * @brief List of all airlines in the world/country.\n\n
 */
void Menu::showAirlines(){
    string option;
    while(true){
        cout << "\n Pretende ver:\n\n "
                "[1] Companhias aéreas totais\n [2] Companhias aéreas de um país\n\n Opção: ";
        cin >> option;
        if (option == "1")
            for (auto i : utilities->getAirlines()){
                printf(BOLD FG_CYAN" %s" RESET_COLOR, i.getCode().c_str());
                cout << " - " << i.getName() << endl;
            }

        else if (option == "2"){
            string country = validateCountry();
            if (country == "0") continue;
            cout << "\n";
            for (auto i : utilities->getAirlines())
                if (i.getCountry() == country){
                    printf(BOLD FG_GREEN" %s" RESET_COLOR, i.getCode().c_str());
                    cout << " - " << i.getName() << endl;
                }
        }
        else if (option == "0")
            return;
        else {
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

/**
 * @brief Shows the first X numbers of airports that exist in each country according to its quantity.
 * @brief List of Airports from a country according to a top(user input) and an order(ascending or descending)\n\n
 */
void Menu::showCountries(){
    string option;
    multimap<int,string> nrAirports = Utils::convertMap(utilities->getNrAirportsPerCountry());
    while(true){
        cout << "\n Pretende ver os países:\n\n "
                "[1] Com mais aeroportos\n [2] Com menos aeroportos\n\n Opção: ";
        cin >> option;
        if (option == "1"){
            int choice = showTop(), top, j = 1;
            if (choice == 1) top = 10;
            else if (choice == 2) top = 20;
            else if (choice == 3) top = customTop("\n Selecione um valor para o top: ", 226);
            else continue;
            for (auto & nrAirport : std::ranges::reverse_view(nrAirports)){
                if (top == 0) break;
                printf(BOLD FG_CYAN"\n %i" RESET_COLOR, j);
                cout << ". " << nrAirport.second << " - " << nrAirport.first << " aeroportos\n";
                top--; j++;
            }
        }
        else if (option == "2") {
            int choice = showTop(), top, j = 1;
            if (choice == 1) top = 10;
            else if (choice == 2) top = 20;
            else if (choice == 3) { top = customTop("\n Selecione um valor para o top: ", 226);}
            else continue;
            for (auto &nrAirport: nrAirports) {
                if (top == 0) break;
                printf(BOLD FG_GREEN"\n %i" RESET_COLOR, j);
                cout << ". " << nrAirport.second << " - " << nrAirport.first << " aeroporto(s)\n";
                top--;
                j++;
            }
        }
        else if (option == "0")
            return;

        else {
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

/**
 * @brief Allows the user to check some numbers about airport/s, flights, airlines and articulation points.\n\n
 */
void Menu::statistics() {
    string option;
    while(true){
        cout << "\n Que dados pretende analisar? \n\n "
                "[1] Estatísticas de um aeroporto\n [2] Nº de voos\n [3] Nº de aeroportos \n [4] Nº de companhias\n [5] Nº de pontos de articulação\n\n Opção: ";

        cin >> option;
        if (option == "1")
            airportStats();

        else if (option == "2")
            numberFlights();

        else if (option == "3")
            numberAirports();

        else if (option == "4")
            numberAirlines();

        else if (option == "5"){
            chooseAirlines(false);
            auto res = utilities->getGraph().articulationPoints(airlines);
            cout << "\n Existem" ;
            printf(BOLD FG_CYAN" %lu " RESET_COLOR, res.size()) ;
            cout << "pontos de articulação\n";
            airlines.clear();
        }

        else if (option == "0") {
            cout << "\n";
            return;
        }

        else{
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

/**
 * @brief Calculates the number of flights according to the users preference.
 * @brief Total flights or flights from an airport.\n\n
 */
void Menu::numberFlights(){
    string option;
    while(true){
        cout << "\n Pretende ver o número de:\n\n "
                "[1] Voos totais\n [2] Voos de uma companhia aérea\n\n Opção: ";
        cin >> option;
        if (option == "1"){
            cout << "\n Existem ";
            printf(BOLD FG_RED"%d " RESET_COLOR, utilities->nrFlights());
            cout << "voos no total\n";
        }
        else if (option == "2"){
            string airline = validateAirline();
            if (airline == "0") continue;
            cout << "\n A " << airline << " tem ";
            printf(BOLD FG_MAGENTA"%d " RESET_COLOR, utilities->getGraph().airlineFlights(airline));
            cout << "voos\n";
        }
        else if (option == "0")
            return;
        else{
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

/**
 * @brief Calculates the numbers of airports according to the user selected option.
 * @brief Total number of airports in the world,country and reachable airports within Y flights; \n\n
 */
void Menu::numberAirports() {
    string option;
    while(true){
        cout << "\n Pretende saber quantos aeroportos:\n\n "
                "[1] Existem no total\n [2] Existem num país\n\n Opção: ";
        cin >> option;
        if (option == "1") {
            cout << "\n Nº de aeroportos totais: ";
            printf(BOLD FG_RED"%lu \n" RESET_COLOR, utilities->getAirports().size());
        }
        else if (option == "2"){
            string country = validateCountry();
            if (country == "0") continue;
            int count = utilities->getNrAirportsPerCountry()[country];

            cout << "\n Neste país (" << country << ") existem ";
            printf(BOLD FG_RED"%i" RESET_COLOR, count);
            cout << " aeroportos\n";
        }

        else if (option == "0")
            return;

        else{
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

/**
 * @brief Total number of airlines in the world,country. \n\n
 */
void Menu::numberAirlines() {
    string option;
    while(true){
        cout << "\n Pretende ver o número de:\n\n "
                "[1] Companhias aéreas totais\n [2] Companhias aéreas de um país\n\n Opção: ";
        cin >> option;
        if (option == "1") {
            cout << "\n Nº de companhias aéreas totais: ";
            printf(BOLD FG_RED"%lu\n" RESET_COLOR, utilities->getAirlines().size());
        }
        else if (option == "2") {
            string country = validateCountry();
            if (country == "0") continue;
            cout << "\n Nº de companhias aéreas: ";
            printf(BOLD FG_MAGENTA"%d\n" RESET_COLOR, utilities->countAirlinesPerCountry(country));
        }
        else if (option == "0")
            return;

        else{
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

/**
 * @brief Gives the user the possibility to get the numbers of a specific airport.
 * @brief Total amount of flights, airlines, reachable cities/airports/countries and total amount of airports/cities/countries within Y flights\n\n
 */
void Menu::airportStats() {
    string option;
    while(true){
        cout << "\n A partir de um aeroporto, pretende saber: \n\n"
                " [1] Nº de voos existentes\n [2] Nº de companhias aéreas\n [3] Nº de cidades alcançáveis\n [4] Nº de aeroportos alcançáveis\n"
                " [5] Nº de países atíngiveis\n [6] Nº de aeroportos/cidades/países possíveis de alcançar com um máximo de Y voos"
                "\n\n Opção: ";
        cin >> option;
        string airport;
        int source;

        if (option == "1"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = utilities->getMap()[airport];
            cout << "\n Nº de voos existentes a partir de " << airport << ":";
            printf(BOLD FG_CYAN" %lu \n" RESET_COLOR, utilities->getGraph().getVertexSet()[source]->getAdj().size()) ;
        }

        else if (option == "2"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = utilities->getMap()[airport];
            cout << "\n Nº de companhias aéreas de " << airport << ":";
            printf(BOLD FG_CYAN" %lu \n" RESET_COLOR, utilities->getGraph().airlinesFromAirport(source).size());
        }

        else if (option == "3"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = utilities->getMap()[airport];
            cout << "\n Nº de destinos distintos alcancáveis a partir de " << airport << ":";
            printf(BOLD FG_CYAN" %lu \n" RESET_COLOR, utilities->getGraph().targetsFromAirport(source).size());
        }

        else if (option == "4"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = utilities->getMap()[airport];
            cout << "\n Nº de aeroportos distintos alcancáveis a partir de " << airport << ":";
            printf(BOLD FG_CYAN" %lu \n" RESET_COLOR, utilities->getGraph().airportsFromAirport(source).size());
        }
        else if (option == "5"){
            airport = validateAirport();
            if (airport == "0") continue;
            source = utilities->getMap()[airport];
            cout << "\n Nº de países diferentes alcancáveis a partir de " << airport << ":";
            printf(BOLD FG_CYAN" %lu \n" RESET_COLOR, utilities->getGraph().countriesFromAirport(source).size());
        }
        else if (option == "6") maxReach();
        else if (option == "0") return;
        else{
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

/**
 * @brief Allows the user to select the max limit of flights from a certain airport/city/coordinates.
 */
void Menu::maxReach() {
    string option;
    string airport = validateAirport();
    if (airport == "0") return;
    int maxFlight = customTop(" Que número máximo de voos pretende realizar: ", 63832);
    while(true){
        cout << "\n O que pretende ver?\n\n"
                " [1] Aeroportos\n [2] Cidades\n [3] Países\n\n Opção: ";
        cin >> option;

        if (option == "1") {
            auto airports = utilities->getGraph().listReachableEntities<Airport::AirportH>
                    (utilities->getMap()[airport],  maxFlight);
            cout << "\n A partir de " << airport << " é possível alcançar o(s) seguinte(s) aeroporto(s)" << "\n\n";
            for(const auto& airport_ : airports){
                printf(BOLD FG_GREEN" %s " RESET_COLOR, airport_.getCode().c_str());
                cout << "- " << airport_.getName() << '\n';
            }
            cout << "Nº de aeroportos alcançáveis ";
            printf(BOLD FG_CYAN" %lu \n" RESET_COLOR, airports.size());
        }
        else if (option == "2") {
            auto cities = utilities->getGraph().listReachableEntities<Airport::CityH2>
                    (utilities->getMap()[airport], maxFlight);

            cout << "\n A partir de " << airport << " é possível alcançar a(s) seguinte(s) cidades(s)" << "\n\n";
            for(const auto& city : cities){
                printf(BOLD FG_GREEN" %s " RESET_COLOR, city.second.c_str());
                cout << "- "<< city.first << '\n';
            }
            cout << "Nº de cidades alcançáveis ";
            printf(BOLD FG_CYAN" %lu \n" RESET_COLOR, cities.size());
        }
        else if (option == "3"){
            auto countries = utilities->getGraph().listReachableEntities<std::set<std::string>>
                    (utilities->getMap()[airport], maxFlight);

            cout << "\n A partir de " << airport << " é possível alcançar o(s) seguinte(s) países(s)" << "\n\n";
            for(const auto& country : countries){
                printf(BOLD FG_GREEN" - " RESET_COLOR);
                cout << " " << country << '\n';
            }
            cout << "Nº de paises alcançáveis ";
            printf(BOLD FG_CYAN" %lu \n" RESET_COLOR, countries.size());
        }
        else if (option == "0")
            return;

        else {
            cout << "\n Input inválido, tente novamente. \n";
            cin.clear();
            cin.ignore(INT_MAX, '\n');
        }
    }
}

/**
 * @brief Asks the user to choose between a predefined top(10/20) or to choose a specific top.
 * @return chosen top
 */
int Menu::showTop(){
    cout << "\n Deseja consultar:\n\n "
            "[1] Top 10\n [2] Top 20\n [3] Outro\n\n Opção: ";
    int option;
    cin >> option;
    while (cin.fail() || option < 0  || option > 4){
        cout << " Input inválido\n Tente novamente: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> option;
    }
    return option;
}

/**
 * @brief User can decide the size of the top he wants to see.\n\n
 * @param message - message chosen from developers
 * @param n - maximum range for top
 * @return top size
 */
int Menu::customTop(const string& message, int n) {
    cout << message;
    int option; cin >> option;
    while (cin.fail() || option < 0 || option > n){
        cout << " Escolha um número entre 1 e " << n << "\n Tente novamente: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> option;
    }
    return option;
}

/**
 * @brief Validation of input of the user for airport code.
 * @return airport code if it is valid.
 */
string Menu::validateAirport(){
    string airport;
    cout << " Insira o código IATA do aeroporto (ex: CDG): "; cin >> airport;

    while(cin.fail() || !utilities->isAirport(Airport(airport))) {
        if (airport == "0") return "0";
        if (cin.fail() || airport.size() != 3) cout << " Input inválido " << '\n';
        else cout << " Não existe nenhum aeroporto com este código " << '\n';
        cout << " Insira o código IATA do aeroporto (ex: CDG): ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> airport;
    }
    return airport;
}

/**
 * @brief Validation of input of the user for airline code.
 * @return  airline code if it is valid.
 */
string Menu::validateAirline() {
    string airline;
    cout << " Insira o código ICAO da companhia aérea (ex: IBE): "; cin >> airline;

    while(cin.fail() || !utilities->isAirline(Airline(airline)) || airlines.find(Airline(airline)) != airlines.end()) {
        if (airline == "0") return "0";
        if (cin.fail() || airline.size() != 3) cout << " Input inválido " << '\n';
        else if (!utilities->isAirline(Airline(airline))) cout << " Não existe nenhuma companhia aérea com este código " << '\n';
        else cout << " Já inseriu esta companhia aérea !\n";
        cout << " Insira o código ICAO da companhia aérea (ex: IBE): ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> airline;
    }
    return airline;
}

/**
 * @brief Validation of input of the user for country.
 * @return country if it is indeed valid
 */
string Menu::validateCountry(){
    string country;
    cout << " Insira o nome do país (ex: Portugal): ";
    cin.ignore();

    getline(cin,country,'\n');

    while(!utilities->isCountry(country)) {
        if (country == "0") return "0";
        cout << " Este país não existe\n";
        cout << " Insira o nome do país (ex: Portugal): ";
        cin.clear();
        getline(cin,country,'\n');
    }
    return country;
}

/**
 * @brief Validation of input of the user for city.
 * @param country
 * @return  city if it is valid.
 */
string Menu::validateCity(const string& country) {
    string city;
    cout << " Insira o nome da cidade (ex: Porto): ";

    getline(cin,city,'\n');

    while(cin.fail() || !utilities->isValidCity(country,city)) {
        if (city == "0") return "0";
        if (!utilities->isCity(city)) cout << " Esta cidade não existe\n";
        else cout << " Esta cidade não pertence a este país :/\n";
        cout << " Insira o nome da cidade (ex: Porto): ";
        cin.clear();
        getline(cin,city,'\n');
    }
    return city;
}

/**
 * @brief Validation of input of the user for latitude.
 * @return  latitude if it is within its range/valid.
 */
double Menu::validateLatitude() {
    double latitude;
    cout << " Insira o valor da latitude: "; cin >> latitude;
    while(cin.fail() || latitude < -90 || latitude > 90) {
        if (cin.fail()) cout << " Input inválido\n";
        else cout << " Insira um valor entre -90 e 90\n";
        cout << " Insira o valor da latitude: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> latitude;
    }
    return latitude;
}

/**
 * @brief Validation of input of the user for longitude.
 * @return  longitude if it is within its range/valid.
 */
double Menu::validateLongitude() {
    double longitude;
    cout << " Insira o valor da longitude: "; cin >> longitude;
    while(cin.fail() || longitude < -180 || longitude > 180) {
        if (cin.fail()) cout << " Input inválido\n";
        else cout << " Insira um valor entre -180 e 180\n";
        cout << " Insira o valor da longitude: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> longitude;
    }
    return longitude;
}

/**
 * @brief Validation of input of the user for radius.
 * @return  radius if it is within its range/valid.
 */
double Menu::validateRadius() {
    double radius;
    cout << " Insira o valor do raio: "; cin >> radius;
    while(cin.fail() || radius <= 0) {
        if (cin.fail()) cout << " Input inválido\n";
        else cout << " Insira um valor positivo\n";
        cout << " Insira o valor do raio: ";
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> radius;
    }
    return radius;
}

/**
 * @brief Validation of input of the user for location.
 * @return vector of codes of airports that exist in that range
 */
vector<string> Menu::validateLocal() {
    double latitude = validateLatitude();
    double longitude = validateLongitude();
    double radius = validateRadius();
    vector<string> local = utilities->localAirports(latitude,longitude,radius);
    while (local.empty()){
        cout << " Não existem aeroportos no local indicado, tente novamente\n";
        latitude = validateLatitude();
        longitude = validateLongitude();
        radius = validateRadius();
        local = utilities->localAirports(latitude,longitude,radius);
    }
    return local;
}

/**
 * @brief Verifies if the option selected by the user is valid or not
 * @param message - message chosen from developers
 * @return user's option
 */
string Menu::validateOption(const string &message) {
    string option;
    cout << message; cin >> option;
    while(!(option == "0" || option == "1" || option == "2")) {
        cout << "\n Input inválido" << '\n';
        cout << message;
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cin >> option;
    }
    return option;
}