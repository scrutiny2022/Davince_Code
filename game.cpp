//达芬奇密码 C++版
//缺： 电脑猜牌的逻辑computer.guesscard(player)
#include<iostream>
#include<vector>
#include<string>
#include<iterator>
#include<algorithm>
#include<ctime>
#include<cstdlib>

struct card_property
{
    int value;
    char color;
    int status = 0; //status为1表示牌倒下，被猜中了;0则表示未被猜中
};
typedef std::vector<card_property> Vector;
void swap(card_property &card1, card_property & card2)
{
    card_property temp;
    temp.color = 'y';
    temp.value = 12;
    temp = card1;
    card1 = card2;
    card2 = temp;
}
class Player{
private :
    Vector hand; //手中的牌
    char character; //p代表玩家，c代表电脑
public :
    Player() {}
    Player(char charact){
        character = charact;
    }
    void drawcardfromDeck(char &color_wanted); //从牌堆中抽牌
    void print();//输出手中的牌
    void sortHand();//将手中的牌排序
    void guessCard(Player &opponent);//猜对手的牌
    Vector & getHand(){return hand;}//返回一个Vector hand的引用
};

class Deck{
    //牌堆类，生成白牌和黑牌
private:
    Vector cardlist;
public:
    void create_vector(char color){
        card_property t;
        t.color = color;
        for ( int i = 0; i < 12; ++i)
        {
            t.value = i;
            cardlist.push_back(t);
        }
    }
    void create_empty_vector(void){
        bool isEmpty = cardlist.empty();
        if ( !isEmpty) cardlist.clear();
    }
    void print(void){
        for (const auto& card : cardlist)
        {
        std::cout<<card.value<<"\t"<<card.color<<std::endl;
        }
    }
    Vector & getCardlist(){return cardlist;}//返回一个Vector cardlist的引用
    //void dealCards(Player &player);//发牌
};

Deck Black,White; //将黑白两牌堆设置成全局变量

void remainDeck(Deck &whichdeck, const card_property &cardTaken) {
    //@param:第一个参数是牌堆，cardTaken是要去掉的那张牌
    //@return: 返回的是去掉cardTaken这张牌之后的牌堆
    //要想使得remainDeck(c,cardTaken)可以运行，第一个参数就得是Deck类，并在函数体的第一步得到Vector的引用
    //所以要加一句 Vector & deck = whichdeck.getCardlist();
    //从牌堆中减掉一张牌
    Vector & deck = whichdeck.getCardlist();
    auto it = std::find_if(deck.begin(), deck.end(), [&](const card_property& card) {
        return card.value == cardTaken.value && card.color == cardTaken.color;
    });

    if (it != deck.end()) {
        deck.erase(it);
    }
}

bool compareProperties(const card_property& prop1, const card_property& prop2) {
    //如果prop2排在prop1后面，也就是说prop2 > prop1, 返回 true
    if (prop1.value < prop2.value) {
        return true;
    } else if (prop1.value == prop2.value) {
        if (prop1.color == 'b' && prop2.color == 'w') {
            return true;
        }
    }
    return false;
}

void Player::drawcardfromDeck(char &color_wanted){
    //在使用前调用srand(time(0))
    card_property temp;
    temp.value = rand() % 12;
    temp.color = color_wanted;
    std::cout<<temp.status<<","<<temp.value<<std::endl;
    if (color_wanted == 'b') remainDeck(Black,temp);
    if (color_wanted == 'w') remainDeck(White,temp);
    hand.push_back(temp);
}
void Player::print(){
    for (const auto& card : hand)
    {
        if (character == 'c') //如果电脑牌被猜中了，则显示数字；反之，不显示
        {
            if (card.status) std::cout<<card.value<<"\t"<<card.color<<std::endl;
            else std::cout<<' '<<"\t"<<card.color<<std::endl;
        }
        if (character == 'p')//如果玩家牌被猜中了，则多4个空格；反之，不变
        {
            if (!card.status) std::cout<<card.value<<"\t"<<card.color<<std::endl;
            else std::cout<<"\t"<<card.value<<"\t"<<card.color<<std::endl;
        }
    }
}
void Player::sortHand(){
    //将牌堆按照从小到大，相同数字白比黑大的规则排列
    bool result = true;
    int i = 0, j = 1;
    card_property temp;
    for(i = 0; i < hand.size()-1; i ++)
    {
        for (j = i+1; j < hand.size(); j++)
        {
            result = compareProperties(hand[i],hand[j]);
            if (!result) swap(hand[i],hand[j]);
        }
    }
}
void Player::guessCard(Player &opponent)
{
    //猜对方的牌
    card_property temp;
    Vector & opponent_hand = opponent.getHand();
    int index = 0;
    bool continue_guessing = true;
    std::cin>>continue_guessing;
    while (continue_guessing)
    {
        std::cout<<"Please input the index of the card(start from 0) you want to guess"<<std::endl;
        std::cin>>index;
        std::cout<<"Please input the value you guess"<<std::endl;
        std::cin>>temp.value;
        temp.color = opponent_hand[index].color;
        if (temp.value == opponent_hand[index].value) opponent_hand[index].status = 1;
        else hand[hand.size()-1].status = 1;//倒下自己新拿的牌
        std::cout<<"continue or not? 1 for continue and 0 for not"<<std::endl;
        std::cin>>continue_guessing;
    }

}
void dealCards(Player &player){
    //发牌
    char b = 'b', w = 'w';
    player.drawcardfromDeck(b);
    player.drawcardfromDeck(w);
    player.drawcardfromDeck(b);
    player.drawcardfromDeck(w);
    player.print();
}

class Process{
private:
    Player player;
    Player computer;
public:
    void setup() {
        player = Player('p');
        computer = Player('c');
    }
    void run();
};
void Process::run()
{
    dealCards(player);
    dealCards(computer);
    char color_wanted;
    std::cout<<"what color you want to draw"<<std::endl;
    std::cin>>color_wanted;
    player.drawcardfromDeck(color_wanted);
    player.guessCard(computer);
    computer.drawcardfromDeck(color_wanted);
    computer.guessCard(player);
}
int main(void)
{
    Black.create_empty_vector(); //Black是黑色牌堆
    Black.create_vector('b');
    //Black.print();
    White.create_empty_vector(); //a是白色牌堆
    White.create_vector('w');
    //White.print();
    std::cout<<"after removing"<<std::endl;
    card_property b,d;
    b.color = 'b';
    b.value = 3;
    d.color = 'w';
    d.value = 6;
    remainDeck(Black,b);
    //Black.print();
    Player player('p');
    srand(time(0));
    dealCards(player);
    //White.print();
    //Black.print();
    std::cout<<"computer's hand is"<<std::endl;
    player.sortHand();
    player.print();
    return 0;
}
