#include <iostream>


template<typename T = std::string , typename... Args>
struct dataContainer{

    dataContainer(std::string& filename , Args ...args){
        
    }

    private:
        void readLine(T first , Args ..args){
            
        }

};

struct mlModel {


    void getData(){
        // get data for model
    }

    void splitData(){
        // split data from model
    }
    
    void Fit(){
        // train model
    }

    void Predict(){
        // predict model
    }

    void predictError(){
        // MSE or something 
    }
};


struct LinerRegression:mlModel{

};



int main() {

    return 0;
}