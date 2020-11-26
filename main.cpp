#include <algorithm>
#include <iostream>
#include "stack.h"
#include "rhombus.h"
#include "allocator.h"


void menu(){
    std::cout << std::endl << "Enter number for action:" << std::endl;
    std::cout << "1 Add Rhombus to stack" <<
    std::endl << "2 Remove item" <<
    std::endl << "3 Show all Rhombuses with std::for_each" <<
    std::endl << "4 Show top stack element" <<
    std::endl << "5 Display on screen number of objects whose area is\n\tless than specified with std::count_if" <<
    std::endl << "6 Print Menu" <<
    std::endl << "0 Exit" << std::endl;
}


int main(){
    int index, threshold_area, cmd = 6;
    Stack<Rhombus<double>, allocator<Rhombus<double>, 128>> st;
    double area;
    do {
		menu();
		std::cin >> cmd;
		switch(cmd) {
            case 1: {
                Rhombus<double> rhom;
			    std::cout << "Add item to top of stack - 1\n\t to iterator position - 2" << std::endl;
			    std::cin >> cmd;
                if(cmd == 1) {
				    std::cout << "push --> Input points: ";
                    try {
					    std::cin >> rhom;
                        st.push(rhom);
		            } catch (std::exception &e) {
					    std::cout << e.what() << std::endl;
					    break;
				    }
					continue;
                }
			    else if (cmd == 2) {
					std::cout << "insert --> Input points: ";
					try {
						std::cin >> rhom;
					} catch (std::exception &e) {
						std::cout << e.what() << std::endl;
						break;
					}
					std::cout << "       --> Input index: ";
					std::cin >> index;
					try {
                        auto it = st.begin();
                        for (size_t i = 0; i < index; ++i)
                            ++it;
                        st.insert(it, rhom);
					} catch (std::bad_alloc &e) {
						std::cout << e.what() << std::endl;
						break;
					}
					continue;
				}
				else {
					std::cout << "[Error 1] Invalid input" << std::endl;
					std::cin.clear();
					std::cin.ignore(30000, '\n');
					break;
				}
			}
			case 2: {
				std::cout << "Delete item from top on stack - 1\n\t to iterator position - 2" << std::endl;
				std::cin >> cmd;
				if (cmd == 1) {
					std::cout << "pop -->";
                    try {
					    st.pop();
                    } catch(std::logic_error &e){
                        std::cout << e.what() << std::endl;
					    break;
                    }
                    continue;
				}
				else if(cmd == 2) {
					std::cout << "erase --> Input index: ";
					std::cin >> index;
					try {
                        if(index < 0 || index > st.size)
                            throw std::logic_error("\nOut of bounds\n");
                        auto it = st.begin();
                        for(size_t i = 0; i < index; ++i)
                            ++it;
                        st.erase(it);
                    } catch(std::logic_error &e) {
                        std::cout << e.what() << std::endl;
				    	break;
                    }
                    continue;
				}
				else {
					std::cout << "[Error 2] Invalid input" << std::endl;
					std::cin.clear();
					std::cin.ignore(30000, '\n');
					break;
				}
			}
			case 3: {
				std::cout << "print -->" << std::endl;
                index = 0;
				std::for_each(st.begin(), st.end(), [&index](auto it){
                    std::cout << index << "\t";
                    it.print(std::cout);
                    std::cout << "\tRhombus area:\t" << it.area() << std::endl;
                    ++index;
                });
				continue;
			}
			case 4: {
				std::cout << "top print --> ";
                try{
                    auto it = st.top();
                    it.print(std::cout);
                } catch(std::logic_error &e){
                    std::cout << e.what() << std::endl;
                    break;
                }
				continue;
			}
		 	case 5: {
				std::cout << "<count_if> --> ";
                std::cout << "Enter area:\t" << std::endl;
                std::cin >> area;
                std::cout << "The number of figures with area less than entered:\t"
                << std::count_if(
                    st.begin(),
                    st.end(),
                    [area](Rhombus<double> thom){
                        return thom.area() < area;
                    }) << std::endl;
				continue;
			}
			case 6: {
                menu();
				continue;
			}
            case 0: {
                return 0;
            }
			default: {
				std::cout << "[Error 3] Invalid input" << std::endl;
				std::cin.clear();
				std::cin.ignore(30000, '\n');
			}
		}
    } while(true);
    return 0;
}
