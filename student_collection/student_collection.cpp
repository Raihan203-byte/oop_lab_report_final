#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

class Student{
    static int cnt;
    int roll;
    std::string name;
    double score;
    public :
    Student(){
        cnt++;
        roll = 26*100+cnt;
    }
    void setdata(){
        std::cout<<"Enter the name of the student : ";
        std::cin>>name;
        std::cout<<"Enter the score of the student : ";
        std::cin>>score;
    }
    int getroll() const{
        return roll;
    }
    void display() const{
        using namespace std;
        cout<<"------------------------\n";
        cout<<"Name : "<<name<<'\n';
        cout<<"Roll : "<<roll<<'\n';
        cout<<"Score : "<<score<<'\n';
        cout<<"------------------------\n";
    }
};

int Student::cnt = 0;

bool descending_cmp(const Student& a, const Student& b) {
    return (a.getroll() < b.getroll());
}




class student_collection{
    std::vector<Student> v;

    public :
        int size(){
            return v.size();
        }
        Student* with(int roll) {
            int ind = -1;
            for(int i = 0 ; i < v.size(); i++){
                if(v[i].getroll() == roll){
                    ind = i;
                    break;
                }
            }
            // if(ind == -1){
            //     std::cout<<"Invalid roll\n";
            //     return NULL;
            // }
            try{
                return &(v.at(ind));
            }
            catch(std::out_of_range e){
                std::cout<<"accessed element is out of bounds\n";
                return NULL;
            }
        }
        void remove(int roll){
            int ind = -1;
            for(int i= 0 ; i < v.size(); i++){
                if(v[i].getroll() == roll){
                    ind = i;
                    break;
                }
            }
            if(ind == -1){
                std::cout<<"Invalid roll entered\n";
                return;
            }
            try{
                v.erase(v.begin() + ind);
            }
            catch(std::out_of_range e){
                std::cout<<"Invalid Roll no.\n";
            }
        }
        void sort_descending(){
            sort(v.begin(), v.end(), descending_cmp);
        }
        student_collection& combine(const student_collection& a, const student_collection& b) const {
            student_collection *s = new student_collection();
            for(const Student& it : a.v){
                (s->v).push_back(it);
            }
            for(const Student& it : b.v){
                (s->v).push_back(it);
            }
            return (*s);
        }
        void add(){
            Student s;
            s.setdata();
            v.push_back(s);
        }

        void display_all() const {
            if(v.empty()){
                std::cout<<"Collection is empty!\n";
                return;
            }

            for(const Student& s : v){
                s.display();
            }
        }
};




void printInstructions(){
    using namespace std;
    cout<<"Enter 1 to quit\n";
    cout<<"Enter 2 to add a new student\n";
    cout<<"Enter 3 to lookup a student\n";
    cout<<"Enter 4 to delete a student\n";
    cout<<">>";
}


int main(){
    std::vector<student_collection> collections;
    int ch;

    while(true){
        printInstructions();
        std::cin>>ch;

        switch(ch){

            case 1:
                return 0;

            case 2:
            {
                collections.emplace_back();
                std::cout<<"Collection created successfully!\n";
                std::cout<<"Collection ID: "<<collections.size()-1<<'\n';
                break;
            }

            case 3:
            {
                int id;
                std::cout<<"Enter collection ID: ";
                std::cin>>id;

                if(id < 0 || id >= collections.size()){
                    std::cout<<"Invalid collection ID!\n";
                    break;
                }

                // Collection-specific menu
                student_collection& selected = collections[id];

                int op;
                while(true){
                    std::cout<<"\n1. Add student\n";
                    std::cout<<"2. Lookup student\n";
                    std::cout<<"3. Remove student\n";
                    std::cout<<"4. Display all students\n";
                    std::cout<<"5. Sort students\n";
                    std::cout<<"6. Back to main menu\n";
                    std::cout<<">> ";

                    std::cin>>op;

                    switch(op){
                        case 1:
                            selected.add();
                            break;

                        case 2:
                        {
                            int roll;
                            std::cout<<"Enter roll: ";
                            std::cin>>roll;

                            Student* s = selected.with(roll);

                            if(s) s->display();
                            else std::cout<<"Student not found!\n";

                            break;
                        }

                        case 3:
                        {
                            int roll;
                            std::cout<<"Enter roll: ";
                            std::cin>>roll;

                            selected.remove(roll);
                            break;
                        }

                        case 4:
                            selected.display_all();
                            break;

                        case 5:
                            selected.sort_descending();
                            break;

                        case 6:
                            goto main_menu;

                        default:
                            std::cout<<"Invalid choice!\n";
                    }
                }

                main_menu:
                break;
            }

            case 4:
            {
                int a, b;

                std::cout<<"Enter first collection ID: ";
                std::cin>>a;

                std::cout<<"Enter second collection ID: ";
                std::cin>>b;

                if(a < 0 || b < 0 ||
                   a >= collections.size() ||
                   b >= collections.size()){
                    std::cout<<"Invalid collection ID!\n";
                    break;
                }

                collections.push_back(
                    collections[0].combine(collections[a], collections[b])
                );

                std::cout<<"Combined collection created!\n";
                std::cout<<"New collection ID: "
                         <<collections.size()-1<<'\n';

                break;
            }

            case 5:
                std::cout<<"Total collections: "
                         <<collections.size()<<'\n';
                break;

            default:
                std::cout<<"Invalid choice!\n";
        }
    }
}
