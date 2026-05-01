#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <array>
#include <variant>
#include <typeinfo>



struct csvData{};

using double_str = std::variant<std::string , double>;

struct DataContainer{

    DataContainer(const std::string& filename){
        std::ifstream file(filename);
        std::string line;
        int colNumb {0};

        // Check if the file is open
        if (!file.is_open()) {
            std::cerr << "Could not open the file!" << std::endl;
            return ;
        }
        int headerEle{0};
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string value;
            std::vector<double_str> row;
            
            // Split the line by commas
            while (std::getline(ss, value, ',')) {
                if(headerEle == 0){
                    this->headerValues[colNumb++] = value;
                }else{
                    if(isNumber(value))
                        row.push_back(value);
                    else
                        row.push_back(std::stod(value));
                }
                
            }
            if(headerEle == 0){
                for(const auto& headerfield : headerValues){
                    if(headerfield !="") this->headerLength++;
                }
            }
            headerEle = 1;
            
            if(row.size() !=0){
                // const auto& field : row
                for (int i = 0;  i < row.size() ; i ++) {
                    std::cout << row[i] << " | ";
                }
            }
            

        }
        
    }

    void getInnerValues(){
        for( const auto& field : this->headerValues){
            std::cout << field << '\n';
        }
    }

    private:
        // fasted option to access . All in stack
        std::array<std::string,15> headerValues{"", "", "", "", "", "", "", "", "", "","","","","",""};
        int headerLength {0};
        // using dataValue = std::variant<std::string , double>;
        using headerFields = std::vector<
            std::vector<
                std::string
            >
        >;
        headerFields headerFieldValues;

        bool isNumber(const std::string& s) {
            try {
                std::stod(s);
                return true;
            } catch (...) {
                return false;
            }
        }


};

struct MlModel {


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


struct LinerRegression:MlModel{

};



int main() {

    DataContainer dataContainer{"../data/regressionData/house.csv"};
    return 0;
}