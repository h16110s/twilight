#ifndef __SCENEDATA__
#define __SCENEDATA__
#include <Arduino.h>


class SceneData{
private:
    static const uint8_t size = 8;              
    static const int num = 4;            //デバイスの数
    byte data[num][size];                //シーンデータ

public:
    String UID;
    SceneData(String UID){
        this -> UID = UID;
    }

    SceneData(String UID, byte* sceneData){
        this -> UID = UID;
        for(int i = 0 ; i < num ; i++){
            setSceneData(i, sceneData[i]);
        }
    }
    
    void setSceneData(int deviceNum, byte *sceneData){
        for(int i = 0; i < size ; i++){
            data[deviceNum][i] = sceneData[i];
        }
    }
    
    void getSceneData(int deviceNum, byte* a){
        for(int i = 0; i < size ; i++){
            a[i] = data[deviceNum][i];
        }
    }

};




#endif