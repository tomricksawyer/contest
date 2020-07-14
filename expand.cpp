#include <iostream>
#include <vector>
#include "class.hpp"

std::vector<Point2D> pList;
std::vector<Point2D> dpList;
std::vector<Point2D> ndpList;
std::vector<Point2D> newList;
void initPList(Index *res){
    /*Index *last = res->prev;
    Index *cur = res;
    Index *next = res ->next;
    while(true){
        pList.push_back(Point2D(cur->Lptr->x1,cur->Lptr->y1));
        if(cur->next != nullptr){
            last = cur;
            cur = next;
            next = next->next;
        }
        else{
            break;
        }
    }
    */
    //for test 1 manualy
    pList.push_back(Point2D(-44,9));
    pList.push_back(Point2D(-44,27));
    pList.push_back(Point2D(-41,30));
    pList.push_back(Point2D(-29,30));
    pList.push_back(Point2D(-26,27));
    pList.push_back(Point2D(-26,9));
    pList.push_back(Point2D(-29,6));
    pList.push_back(Point2D(-41,6));
}



//自定義的向量差乘運算符號，


//var pList = [Point2D]()  // 原始頂點坐標，在initPList 函數當中初始化賦值 

//var dpList = [ Point2D ]()  // 邊向量 dpList ［ i ＋ 1 ］－ dpLIst ［ i ］ 在 initDPList 函數當中計算後賦值

//var ndpList = [ Point2D ]()  // 單位化的邊向量， 在initNDPList函數當中計算後膚質，實際使用的時候，完全可以用dpList來保存他們

//var newList = [Point2D]()  // 新的折線頂點，在compute 函數當中，賦值



//初始化頂點隊列
/*
func initPList(){

    pList   = [ Point2D ( 0 , 0 ),

               Point2D ( 0 , 100 ),

               Point2D ( 100 , 100 ),

               Point2D ( 50 , 50 ),

               Point2D ( 100 , 0 ),

             ]

}
*/



// 初始化 dpList  兩頂點間向量差

void initDPList(){
    std::cout << "dpList Calculate" << std::endl;
    int index;
    for (index = 0; index < pList.size();++index){
        dpList.push_back(pList[index == pList.size() - 1 ? 0 : index + 1] - pList[index]);
    }
}

/*func initDPList()-> Void {

    print ( " 計算 dpList" )

    var index : Int

    for index= 0 ; index< pList . count ; ++index{

        dpList . append ( pList[index==pList . count - 1 ? 0 : index+ 1 ] -pList[index]);

        print ( "dpList[ \ ( index )]=( \ (dpList[index].x), \ (dpList[index].y))" )

    }

}
*/

void initNDPList(){
    std::cout << "calculate ndpList" << std::endl;
    int index = 0;
    for (; index < dpList.size();++index){
        ndpList.push_back(dpList[index] * (1.0/std::math.sqrt(dpList[index]*dpList[index])));
    }
}


//初始化ndpList，單位化兩頂點向量差
/*
func initNDPList()-> Void {

    print ( " 開始計算 ndpList" )

    var index= 0 ;

    for ; index< dpList . count ; ++index {

        ndpList . append ( dpList[index] * ( 1.0 / sqrt ( dpList[index]*dpList[index])) );

        print ( "ndpList[ \ ( index )]=( \ (ndpList[index].x), \ (ndpList[index].y))" )

    }

}
*/
void computeLine(int dist){
    std::cout << "Calculate New Vertex" << std::endl;
    int index = 0;
    for (; index < pList.size();++index){
        int startIndex = index == 0 ? pList.size() - 1 : index - 1;
        int endIndex = index;
        //let sina = ndpList[startInd ex] **ndpList[endIndex ]

        //let length = dist / sina

        //let vector = ndpList[endIndex ] -ndpList[startInd ex]

        //point = pList [index] + vector * length

        //newList.append(point);
        auto sina = ndpList[startIndex] *= ndpList[endIndex];
        auto length = dist / sina;
        auto vec = ndpList[endIndex] - ndpList[startIndex];
        auto point = pList[index] + vec * length;
        newList.push_back(point);
    }
}

/*
//計算新頂點， 注意參數為負是向內收縮， 為正是向外擴張

func computeLine(dist: Double )-> Void {

    print ( "開始計算新頂點" )

    var index = 0

    let count = pList . count ;

    for ; index<count; ++index {

        var point: Point2D

        let startIndex = index== 0 ? count- 1 : index- 1

        let endIndex = index

        

        let sina = ndpList[startInd ex] **ndpList[endIndex ]

        let length = dist / sina

        let vector = ndpList[endIndex ] -ndpList[startInd ex]

        point = pList [index] + vector * length

        newList.append(point);

        print ( "newList[ \ ( index )] = ( \ ( point. x ), \ ( point. y ))" )

    }

}
*/
