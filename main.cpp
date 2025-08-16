#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

class Item{
public:
    string name;
    Item(const string& name_) : name(name_) {}

    virtual void showInfo() const{
        cout <<  name << endl;
    }
};

class Material : public Item{//未実装予定
public:
    Material(const string& n) : Item(n) {}
    void showInfo() const{
        cout << name << endl;
    }
};

class Food : public Item{
public:
    string whatheal;
    int amount;
    Food(const string& n, const string& m, int l) : Item(n), whatheal(m), amount(l){}
};

class Weapon : public Item{
public:
    int mindamage, maxdamage;
    Weapon(const string& n, int mindamage_, int maxdamage_) : Item(n), mindamage(mindamage_), maxdamage(maxdamage_){}

    void showInfo() const override{
        cout << name << ":" << mindamage << "~" << maxdamage << endl;
    }
};

class Skill{
public:
    string name;
    int damage, status;
    Skill(const string& name_, int damage_, int status_) : name(name_), damage(damage_), status(status_) {}
};

//staticについて: 共通の変数や関数を使われる。
/*大域変数：違うファイルからもアクセスできる
  static 大域変数：このファイル内ならどこでもおｋ
  class内 static 変数or関数 : class名::変数or関数　のようにして使えば、どこでもおｋ*/

class ItemManager{
public:
    static vector<Weapon*> extractWeapon(const vector<Item*>& inventory){
        vector<Weapon*> n;
        for(size_t i = 0; i < inventory.size(); i++){
            Weapon* weapon = dynamic_cast<Weapon*>(inventory[i]);
            if(weapon){
                n.push_back(weapon);
            }
        }
        return n;
    };
    static vector<Food*> extractFood(const vector<Item*>& inventory){
        vector<Food*> n;
        for(size_t i = 0; i < inventory.size(); i++){
            Food* food = dynamic_cast<Food*>(inventory[i]);
            if(food){
                n.push_back(food);
            }
        }
        return n;
    };
    static vector<Material*> extractMaterial(const vector<Item*>& inventory){
        vector<Material*> n;
        for(size_t i = 0; i < inventory.size(); i++){
            Material* material = dynamic_cast<Material*>(inventory[i]);
            if(material){
                n.push_back(material);
            }
        }
        return n;
    };
};

int intcin(const string n){
    cout << n;
    int num;
    while(!(cin >> num)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');//(読み飛ばし最大文字数, 読み飛ばし終了地点("ではなく'を使う。文字列ではなく１文字を受け付けてるから))
        cout << "\nerror : Invalid input" << endl;
        cout << "Enter again : ";
    }
    return num;
};

struct equalItem { 
    Item* item;
    equalItem(Item* item_) : item(item_){}
    bool operator ()(Item* n){ return n == item; } 
};

class Character { 
public:
    int hp, mp = 5, maxdamage, mindamage, level=1;
    bool weaflag;
    string name;
    vector<Weapon*> WList ;//<Weapon*>にするのはWListに入れたいweaponがItem*とポインタを扱っているから、型を合わせるため。
    vector<Item*> inventory = {new Item("empty"), new Food("HP nts1", "HP", 40), new Food("HP nuts2", "HP", 40), new Food("MP nuts", "MP", 4), new Weapon("cheat sword", 100, 100), new Material("fire eye")};
    vector<Material*> MList ;
    vector<Skill*> skill = {new Skill("fire", 20, 1)} ;
    vector<Food*> FList;



    Character(const string& name_, int hp_, int mindamage_, int maxdamage_) 
        : name(name_), hp(hp_), mindamage(mindamage_), maxdamage(maxdamage_) {}

    int attack(){ 
         int damage = rand() % (maxdamage - mindamage + 1) + mindamage;
         return damage; 
    };

    void getSkill(Skill* n){
        skill.push_back(n);
    }
    void showSkill(){
        if(skill.empty()){
            cout << "You haven't learned";
        }
        else{
            for(int i = 0; i < skill.size(); i++){
                cout << i+1 << ":" << skill[i]->name << endl;
            }
        }
    }
    int useSkill(int n){
        if(mp > 0){
            mp -= 1;
            return skill[n]->damage;
        }
        else{
            cout << "you are out of MP" << endl;
            return -1;
        }
    }

    bool showInventory(){
        if(inventory.empty()){
            cout << "Not have item" << endl;
            return false;
        }

        cout << "\n----------- Inventory -----------" << endl;
        while(true){
            int num = intcin("\n1. Items,  2. close inventory(next battle) : ");

            if(num == 2) return false;
            
            if(num != 1){
                cout << "error : Invalid input" << endl;
                continue;
            }

            do{
                int snum = intcin("\n1. Weapon,  2. Food,  3. Material,  4. back menu : ");

                if(snum == 1){
                    bool have = showWeapon();
                    selectWeapon(have);
                    return true;
                }

                if(snum == 2){
                    bool have = showFood();
                    selectFood(have);
                    return true;
                }

                if(snum == 3){
                    showMaterial();
                    return true;
                }

                if(snum == 4) break;

                if(1 > snum || snum > 4){
                    cout << "error : Invalid Input" << endl;
                    continue;
                }
            } while(true);
        }
    }

    void showMaterial(){
        MList = ItemManager::extractMaterial(inventory);
        cout << "--- Material list ---" << endl;
        if(MList.empty()){
            cout << "Not exist material" << endl;
            return;
        }
        else{
            for(int i = 0; i < MList.size(); i++){
                cout << i+1 << ":" << MList[i]->name;
            }
        }
    }
    void makeSomething(){
        //未実装
    }

    bool showWeapon(){
        WList = ItemManager::extractWeapon(inventory);
        cout << "--- Weapon list ---" << endl;
        if(WList.empty()){
            cout << "Not exist weapon" << endl;
            return false;
        }
        else{
            for(int i = 0; i < WList.size(); i++){
                cout << i+1 << ":" << WList[i]->name << endl;
            }
            return true;
        }
    }
    void selectWeapon(bool have){
        if(have){
             WList = ItemManager::extractWeapon(inventory);
            int num = intcin("\nSelect weapon number : ");

            num -= 1;
            if(num >= 0 && num <= WList.size()){
                cout << WList[num] -> name << " was selected" << endl;
                mindamage = WList[num] -> mindamage;
                maxdamage = WList[num] -> maxdamage;
            }
        }
    }

    bool showFood(){
        FList = ItemManager::extractFood(inventory);
        cout << "\n--- Food list ---" << endl;
        if(FList.empty()){
            cout << "Not found food" << endl;
            return false;
        }
        else{
            for(size_t i = 0; i < FList.size(); i++){
                cout << i+1 << ":" << FList[i]->name << endl;
            }
            return true;
        }
    }
    void selectFood(bool have){
        if(have){
            FList = ItemManager::extractFood(inventory);
            int num = intcin("\nSelect food number : ");
            num -= 1;
        
            if(num >= 0 && num < FList.size()){
                cout << FList[num] -> name << " was used" << endl;
                if(FList[num] -> whatheal == "MP"){
                    cout << "MP recovered : " << FList[num] -> amount << endl;
                    mp += FList[num] -> amount;
                }
                else if(FList[num] -> whatheal == "HP"){
                    cout << "HP recovered : " << FList[num] -> amount << endl;
                    hp += FList[num] -> amount;
                }
                vector<Item*>::iterator it = remove_if(inventory.begin(), inventory.end(), equalItem(FList[num]));
                inventory.erase(it, inventory.end());
            }
        }
    }

    void addItem(Item* item){
        inventory.push_back(item);
        cout << name << "は" << item->name << "を手に入れた" << endl;
    }

    bool isAlive(){ return hp > 0; }

    void showStatus(){ cout << name << "'s HP" << ":" << hp; }
};


class Enemies : public Character{
public:
    vector<Item*> dropItems;
    vector<Skill*> enemySkill;
    Enemies(const string& name_, int hp_, int mindamage_, int maxdamage_, vector<Item*> drops = {}, vector<Skill*> skill = {}) 
        : Character(name_, hp_, mindamage_, maxdamage_), dropItems(drops), enemySkill(skill){}

    Item* drop(){
        return dropItems[0];
    }

    int useEnemySkill(){
        if(!enemySkill.empty()){
            int n;
            n = rand() % skill.size();
            return enemySkill[n]->damage;
        }
        else{
            //とりあえずスキルなしは攻撃入れておく
            int damage = rand() % (maxdamage - mindamage + 1) + mindamage;
            return damage; 
        }
    }
};


bool mainAction(Character* main, Enemies* enemy){
    cout << "Select your action:" << endl;
    cout << " 1. Attack" << endl;
    cout << " 2. useSkill" << endl;
    cout << " 3. Inventory" << endl;
    int num = intcin("Enter your choice : ");
    while(true){
        if(num == 1){
            int damage;
            cout << main->name << " attacks!" << endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));

            damage =  main->attack(); 
            enemy->hp -= damage;

            cout << "It deals " <<  damage << " damage!" << endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return false;
        }

        if(num == 2){
            do{
                main->showSkill();
                int num = intcin("Select skill number(cancel : 0) : ");
                if (num == 0) {
                    cout << "Canceled skill selection.\n";
                    break;
                }

                if (num < 1 || num > main->skill.size()) {
                    cout << "Invalid skill number.\n";
                    continue;
                }

                int damage = main->useSkill(num - 1);
                if (damage == -1) {
                    continue;
                }

                cout << "\n" << main->name << " uses " << main->skill[num - 1]->name << "!" << endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                enemy->hp -= damage;
                cout << "It deals " << damage << " damage!" << endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                return false;
            }while(true);
        }
    }
}



void enemyAction(Character* main, Enemies* enemy){
    int n;
    n = rand() % 10 + 1;
    if(n < 9){
        //9割で通常攻撃
        cout << enemy->name << "'s attacks!" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        int n = enemy->attack();
        cout << "It deals " << n << " damage!\n" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        main->hp -= n;
    }
    else{
        //1割でスキル攻撃
        cout << enemy->name << " uses skill!" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        int n = enemy->useEnemySkill();
        main->hp -= n;
        cout <<"It deals " << n << " damage!\n" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}


void startBattle(Character* main, Enemies* enemy){
    cout << "\n=== A wild " << enemy->name << " appears! ===\n" << endl;
    while(enemy->hp > 0 && main->hp > 0){
        //主人公の行動
        std::this_thread::sleep_for(std::chrono::seconds(2));
        cout << "=== " << main->name << "'s Turn" << "===" << endl;
        cout << "[" << main->name << "'s HP : " <<  main->hp <<" | " << enemy->name << "'s HP : " << enemy->hp << "]\n" << endl;
        mainAction(main, enemy);
        //倒したか判定
        if(enemy->hp <=0){
            cout <<"\n=== " << enemy->name << " was defeated! ===\n" << endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            main->inventory.push_back(enemy->drop());
            cout << enemy->name << " dropped " << enemy->drop()->name << "!" << endl; 
            break;
        }

        //敵の行動
        std::this_thread::sleep_for(std::chrono::seconds(2));
        cout << "\n=== " << enemy->name << "'s Turn" << "===" << endl;
        cout << "[" << main->name << "'s HP : " <<  main->hp <<" | " << enemy->name << "'s HP : " << enemy->hp << "]\n" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        enemyAction(main, enemy);
        //倒されたか判定
        if(main->hp <=0){
            cout <<"\n=== " << main->name << " was defeated.... ===\n" << endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            break;
        }
    }
};


int main(){

    string name;
    cout << "Insert your name : "; cin >> name;
    cout << "Loading now..." << endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    Character mainP(name, 100, 2, 4);

    vector<Enemies> enemy = { Enemies("SLIM", 10, 2, 4, {new Material("Slim liquid")}),
                              Enemies("GOBLIN", 30, 3, 9, {new Weapon("Goblin Club", 3, 9)}),
                              Enemies("ORC", 40, 8, 20, {new Weapon("Orc hand", 8, 20), new Material("Orc blood")}),
                              Enemies("DEMON KAKA", 100, 1, 30, {new Weapon("Demonssword", 1, 30)})};

    for(size_t i = 0; i < enemy.size(); i++){
        if(mainP.isAlive()){
            startBattle(&mainP, &enemy[i]);
            while(mainP.showInventory()){}
        }
        else{
            cout << "\nYou lose...." << endl;
            break;
        }
    }
    
    if(mainP.isAlive()){
        std::this_thread::sleep_for(std::chrono::seconds(2));
        cout << "\n You Win !!" << endl;
    }
    return 0;
}