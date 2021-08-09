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
#define number_implementers_MAX 10
#define number_implementers_MIN 1
#define number_details_MAX 10
#define number_details_MIN 1
#define IMPLEMENTER_CHANCE_ERROR 0
#define IMPLEMENTER_CHANCE_NOANSWER 0
#include <iostream>
#include <random>
//заметить обычные for на что-то покрасивше, через define или range
enum states{
    wait,       //0
    success,    //1
    error = -1,  //-1
    no_answer = -2
};
using namespace std;
class Conveyor {
private:
    class Implementer{
    private:
        int state;
        bool busy;
    public:
        Implementer() {
            state= states(wait);
            busy = false;
        }
        int technological_operation() {
            if (busy) {
                std::random_device rd;
                std::mt19937 mersenne(rd()); // инициализируем Вихрь Мерсенна случайным стартовым числом
                state = ((mersenne() % 100 + 1) <= IMPLEMENTER_CHANCE_NOANSWER) ? states(no_answer) : states(wait);
                if (!(state == states(no_answer))) {
                    state = ((mersenne() % 100 + 1) <= IMPLEMENTER_CHANCE_ERROR) ? states(error) : states(success);
                }
            }
            else{
                state = states(wait);
            }
            return state;
        }
        void show() {
                std::cout<<technological_operation()<<" ";
        }
        void set_busy(bool input_busy) {
            busy=input_busy;
        }
        int get_state() const {
            return state;
        }
    };
    Implementer* ptr = nullptr;
    int number_implementers;
    int number_details;
    int step;
public:
    Conveyor():number_implementers(1), number_details(10), step(0) {
        while (true) {
            std::cout << "Введите количество исполнительных механизмов (участков) в конвейре: " << std::endl;
            if (std::cin >> number_implementers) {
                if ((number_implementers >= number_implementers_MIN) &&
                    (number_implementers <= number_implementers_MAX)) {
                    break;
                } else {
                    std::cout
                            << "Неккоректный ввод, количество исполнительных механизмов (участков) конвейра должно быть в диапазоне: ["
                            << number_implementers_MIN << "," << number_implementers_MAX << "]" << std::endl;
                }
            }
            else{
                std::cin.clear();
                std::cin.ignore(32767, '\n');
                std::cout
                        << "Неккоректный ввод, количество исполнительных механизмов (участков) конвейра должно быть в диапазоне: ["
                        << number_implementers_MIN << "," << number_implementers_MAX << "]" << std::endl;
            }
        }
        while (true) {
            std::cout << "Введите количество деталей для обработки: " << std::endl;
            if (std::cin >> number_details) {
                if ((number_details >= number_details_MIN) &&
                    (number_details <= number_details_MAX)) {
                    break;
                } else {
                    std::cout
                            << "Неккоректный ввод, количество деталей для обработки должно быть в диапазоне: ["
                            << number_details_MIN << "," << number_details_MAX << "]" << std::endl;
                }
            }
            else{
                std::cin.clear();
                std::cin.ignore(32767, '\n');
                std::cout
                        << "Неккоректный ввод, количество исполнительных механизмов (участков) конвейра должно быть в диапазоне: ["
                        << number_details_MIN << "," << number_details_MAX << "]" << std::endl;
            }
        }
        ptr = new Implementer[number_implementers];
        show();
    }
    Conveyor(int input_number_implementers, int input_number_details):number_implementers(input_number_implementers), number_details(input_number_details), step(0) {
        ptr = new Implementer[number_implementers];
        show();
    }
    void process(int temp) {
        do {
            int S=0;
            cout << step << ": ";
            for (int i = 0; i < number_implementers; i++) {
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
                if((ptr[i].get_state() != states(error))&&(ptr[i].get_state() != states(no_answer))) {
                    S++;
                }
            }
            cout <<" S = " << S << endl;
            if (S!=number_implementers) {
                if (!crash()){
                    return;
                } else{
                    repair();
                }
            }
            step++;
        } while (step!=temp);
    }
    void show() const{
        cout<<"Количество ИП = "<<number_implementers <<endl;
        cout<<"Количество Деталей = "<<number_details <<endl;
    }
    void repair() const{
        cout<<"ИП был починен. Над деталью уже выполнили действие" <<endl;
    }
    int crash(){
        int answer = 0;
        cout<<"There was an accident. Hardware error. In step "<<get_step() <<endl;
        while (true) {
            cout << "Continue automatic operation? (1 - Yes, 0 - No) "<<endl;
            if (std::cin >> answer) {
                if ((answer >= 0) &&
                    (answer <= 1)) {
                    break;
                } else {
                    std::cout << "Неккоректный ввод, введите число 1 - Yes, 0 - No"<< std::endl;
                }
            }
            else{
                std::cin.clear();
                std::cin.ignore(32767, '\n');
                std::cout << "Неккоректный ввод, введите число 1 - Yes, 0 - No"<< std::endl;
            }d
        }
        return answer;
    }
    int get_step() const{
        return step;
    }
    ~Conveyor() {
        std::cout << "work of the destructor" << std::endl;
        delete [] ptr;
    }
};

int main(){
    system("chcp 65001");
    int amount_steps = 0;
	Conveyor a(10,10);
    std::cout << "Сколько шагов должен выполнить конвейер?" << std::endl;
    cin >> amount_steps;
	//a.process(amount_steps);
	return 0;
}