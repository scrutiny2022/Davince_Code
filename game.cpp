//达芬奇密码 C++版
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
};

Deck Black,White; //将黑白两牌堆设置成全局变量

void remainDeck(Deck &whichdeck, const card_property &cardTaken) {
    //要想使得remainDeck(c,cardTaken)可以运行，第一个参数就得是Deck类，并在函数体的第一步得到Vector的引用
    //所以要加一句 Vector & deck = whichdeck.getCardlist();
    //从牌堆中减掉一张牌
    //第一个参数是牌堆，cardTaken是要去掉的那张牌
    //返回的是去掉cardTaken这张牌之后的牌堆
    Vector & deck = whichdeck.getCardlist();
    auto it = std::find_if(deck.begin(), deck.end(), [&](const card_property& card) {
        return card.value == cardTaken.value && card.color == cardTaken.color;
    });

    if (it != deck.end()) {
        deck.erase(it);
    }
}


class Player{
private :
    Vector hand; //玩家手中的牌
    char character; //p代表玩家，c代表电脑
public :
    void drawcardfromDeck(char &color_wanted); //从牌堆中抽牌
};
void Player::drawcardfromDeck(char &color_wanted){
    //在使用前调用srand(time(0))
    card_property temp;
    temp.value = rand() % 12;
    temp.color = color_wanted;
    std::cout<<temp.status<<","<<temp.value<<std::endl;
    if (color_wanted == 'b') remainDeck(Black,temp);
    if (color_wanted == 'w') remainDeck(White,temp);
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
    card_property b;
    b.color = 'b';
    b.value = 3;
    remainDeck(Black,b);
    //Black.print();
    Player player;
    srand(time(0));
    char a = 'b', c = 'w';
    player.drawcardfromDeck(a);
    player.drawcardfromDeck(c);
    White.print();
    Black.print();
    return 0;
}
