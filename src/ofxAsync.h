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
    
    class AsyncRunnerWithArg: public ofThread {
    public:
        void setup(std::function<void(ofThread*)> _func){
            this->func = _func;
        }
        
        void threadedFunction(){
            this->func(this);
        }
        
        std::function<void(ofThread*)> func;
    };
    
    static std::map<int, shared_ptr<ofThread> > runners;
    static AsyncExit asyncExit;
    
public:
    static int run(std::function<void()> func);
    static int run(std::function<void(ofThread*)> func);
    static void update();
    static bool exists(int thread_id);
    static bool isRunning(int thread_id);
    static void stop(int thread_id, bool wait_until_stop=true);
    static void stopAll(bool wait_until_stop=true);
};
