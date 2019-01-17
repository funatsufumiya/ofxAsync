#pragma once

#include "ofMain.h"

class ofxAsync {
private:
    class AsyncExit {
    public:
        AsyncExit(){}
        ~AsyncExit(){
            ofxAsync::stopAll();
        }
    };
    
    class AsyncRunner: public ofThread {
    public:
        void setup(std::function<void()> _func){
            this->func = _func;
        }
        
        void threadedFunction(){
            this->func();
        }
        
        std::function<void()> func;
    };
    
    static std::map<int, shared_ptr<AsyncRunner> > runners;
    static AsyncExit asyncExit;
    
public:
    static int run(std::function<void()> func);
    static void update();
    static void stop(int thread_id);
    static void stopAll();
};
