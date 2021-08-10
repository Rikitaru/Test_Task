/*Требуется написать программу управления сборочным конвейером.
Конвейер состоит из движущейся ленты, на которой установлены детали,
и исполнительных механизмов(далее ИМ для краткости).
Конвейер разбит на участки, каждый из которых обслуживает отдельный ИМ.
Технологический цикл работы конвейера включает в себя сдвиг ленты и обработку деталей.
Сдвиг ленты возможен, только если все ИМ сообщили об успешном выполнении операции.
Если ИМ сообщает об аварии или не отвечает в течение заданного времени, то конвейер 
останавливается и регистрируется авария, после чего возврат в автоматический режим 
работы возможен только по команде оператора.
После сдвига ленты, ИМ по команде управляющей программы выполняет одну технологическую
операцию над деталью.После того как все ИМ успешно отработали операцию, технологический цикл повторяется.*/
#define number_implementers_MAX 10      //максимальное количество ИМ в конвейере
#define number_implementers_MIN 1       //минимальное  количество ИМ в конвейере
#define number_details_MAX 10           //максимальное количество Деталей в конвейере
#define number_details_MIN 1            //минимальное  количество Деталей в конвейере
#define IMPLEMENTER_CHANCE_ERROR 2      //вероятность ошибки ИМ при работе с деталью (в процентах)
#define IMPLEMENTER_CHANCE_NO_ANSWER 2  //вероятность проблемы "не отвечает в течение заданного времени" (в процентах)
#include <iostream>
#include <random>
#include <exception>
#include <stdexcept>
#include <iomanip>

enum states{                //состояния для ИМ
    wait,                   //0  ИМ ожидает деталь
    success,                //1  ИМ успешно отработал с деталью
    error = -1,             //-1 ИМ получил ошибку, работая с деталью
    no_answer = -2          //-2 ИП получил проблему "не отвечает в течение заданного времени", работая с деталью
};
class Conveyor {
private:
    class Implementer {
    private:
        int state; //состояние ИМ
        bool busy; //показатель "работает ли на текущем шагу (цикле конвейера) данный ИМ"
    public:
        Implementer() {
            state = states(wait);
            busy = false;
        }

        int get_state() const {
            return state;
        }

        void set_busy(bool input_busy) {
            busy = input_busy;
        }

        void show() {
            std::cout << std::setw(2) << technological_operation() << " ";
        }

        int technological_operation() {
            if (busy) {
                std::random_device rd;
                std::mt19937 mersenne(rd()); // инициализируем Вихрь Мерсенна случайным стартовым числом
                state = ((mersenne() % 100 + 1) <= IMPLEMENTER_CHANCE_NO_ANSWER) ? states(no_answer) : states(wait);
                if (state != states(no_answer)) {
                    state = ((mersenne() % 100 + 1) <= IMPLEMENTER_CHANCE_ERROR) ? states(error) : states(success);
                }
            } else {
                state = states(wait);
            }
            return state;
        }

        ~Implementer() = default;
    };
    uint16_t number_implementers;   //количество ИМ в конвейере
    uint16_t number_details;        //количество Деталей в конвейере
    uint16_t step;                  //Номер Шага (цикла) конвейера
    std::unique_ptr<Implementer[]> ptr = nullptr;   //unique_ptr указатель для безопасной работы конструктора Conveyor(int16_t const input_number_imp...) с throw
public:
    Conveyor() : number_implementers(1), number_details(10), step(0) { //конструктор для ввода с клавиатуры
        while (true) {
            std::cout << "Enter the number_implementers (sections) in the pipeline(Conveyor): " << std::endl;
            if (std::cin >> number_implementers) {
                if ((number_implementers >= number_implementers_MIN) &&
                    (number_implementers <= number_implementers_MAX)) {
                    break;
                } else {
                    std::cout
                            << "Incorrect input, the number_implementers must be in the range: ["
                            << number_implementers_MIN << "," << number_implementers_MAX << "]" << std::endl;
                }
            } else {
                std::cin.clear();
                std::cin.ignore(32767, '\n');
                std::cout
                        << "Incorrect input, the number_implementers must be in the range: ["
                        << number_implementers_MIN << "," << number_implementers_MAX << "]" << std::endl;
            }
        }
        while (true) {
            std::cout << "Enter the number_details to process: " << std::endl;
            if (std::cin >> number_details) {
                if ((number_details >= number_details_MIN) &&
                    (number_details <= number_details_MAX)) {
                    break;
                } else {
                    std::cout
                            << "Incorrect input, the number_details must be in the range: ["
                            << number_details_MIN << "," << number_details_MAX << "]" << std::endl;
                }
            } else {
                std::cin.clear();
                std::cin.ignore(32767, '\n');
                std::cout
                        << "Incorrect input, the number_details must be in the range: ["
                        << number_details_MIN << "," << number_details_MAX << "]" << std::endl;
            }
        }
        ptr = std::unique_ptr<Implementer[]>{new Implementer[number_implementers]};
        show();
    }

    Conveyor(int16_t const input_number_implementers, int16_t const input_number_details) : number_implementers(
            input_number_implementers), number_details(input_number_details), step(0) { //конструктор с параметрами и исключениями
        if (input_number_implementers <= 0 ) {
            throw std::logic_error("Error. The input_number_implementers must be positive");
        }
        if (input_number_details <= 0 ) {
            throw std::logic_error("Error. The input_number_details must be positive");
        }
        ptr = std::unique_ptr<Implementer[]>{new Implementer[number_implementers]};
        show();
    }

    void process(int16_t const input_amount_steps) { //процесс конвейера
        if (input_amount_steps <= 0 ) {
            throw std::logic_error("Error. The input_amount_steps must be positive");
        }
        if (ptr == nullptr) {
            throw std::logic_error("Error. Null ptr");
        }
        do {
            uint16_t S = 0;
            std::cout << std::setw(2) << step << ": ";
            for (uint16_t i = 0; i < number_implementers; i++) { //в зависимости от номера Шага конвейера определяется занятость каждого ИМ
                if (step <= number_details) {
                    if (i < (step % (number_details + 1))) {
                        ptr[i].set_busy(true);
                    }
                } else {
                    if (i < (step - number_details)) {
                        ptr[i].set_busy(false);
                    } else if (i < step) {
                        ptr[i].set_busy(true);
                    }
                }
                ptr[i].show();
                if ((ptr[i].get_state() != states(error)) && (ptr[i].get_state() != states(no_answer))) {
                    S++;
                }
            }
            std::cout << " S = " << S << std::endl;
            if (S != number_implementers) { //проверка на то, что все конвейеры отработали штатно
                if (!crash()) { //вызов оператора
                    std::cout << " pipeline stop" << std::endl;
                    return;
                } else {
                    repair(); //вызов ремонта
                }
            }
            step++;
            if ((step == input_amount_steps)&&(step!=1)&&(S==number_implementers)){
                std::cout << " The pipeline has completed all the work" << std::endl;
            }
        } while (step != input_amount_steps);
    }

    void show() const {
        std::cout << "Number of implementers = " << number_implementers << std::endl;
        std::cout << "Number of details = " << number_details << std::endl;
    }

    void static repair() {
        std::cout << "Implementer as been fixed. An action has already been performed on the part." << std::endl;
    }

    uint16_t crash() const {
        uint16_t answer = 0;
        std::cout << "There was an accident. Hardware error. In step " << get_step() << std::endl;
        while (true) {
            std::cout << "Continue automatic operation? (1 - Yes, 0 - No) " << std::endl;
            if (std::cin >> answer) {
                if ((answer >= 0) &&
                    (answer <= 1)) {
                    break;
                } else {
                    std::cout << "Incorrect input, enter the number 1 - Yes, 0 - No" << std::endl;
                }
            } else {
                std::cin.clear();
                std::cin.ignore(32767, '\n');
                std::cout << "Incorrect input, enter the number 1 - Yes, 0 - No" << std::endl;
            }
        }
        return answer;
    }

    uint16_t get_step() const {
        return step;
    }

    ~Conveyor() {
        std::cout << "work of the destructor" << std::endl;
    }
};

int main() {
    system("chcp 65001");
    int16_t amount_steps = 0;
    std::cout << "How many steps should the pipeline perform?" << std::endl;
    std::cin >> amount_steps;
    try {
        //Conveyor a(10, 4); //Конструктор с параметрами и throw
        Conveyor a;         //Конструктор с пользовательским вводом
        a.process(amount_steps);
    }
    catch (std::exception const &e) {
        std::cerr << e.what() << '\n';
    }
    catch (...) {
        std::cout << "Unknown error" << '\n';
    }
    return 0;
}