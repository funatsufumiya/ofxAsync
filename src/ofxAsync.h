#pragma once

#include "ofMain.h"

#include "boost/optional.hpp"

class ofxAsync {
private:
    class AsyncExit {
    public:
        AsyncExit(){}
        ~AsyncExit(){
            if (wait_for_all_when_exit) {
                try{
                    ofxAsync::stopAll(true);
                }catch(...){}
            }else{
                try{
                    ofxAsync::stopAll(false);
                }catch(...){}
            }
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
    static int thread_id_max;
    static bool wait_for_all_when_exit;
    
public:
    static int run(std::function<void()> func);
    static int run(std::function<void(ofThread*)> func);
    static void update();
    static bool exists(int thread_id);
    static bool isRunning(int thread_id);
    static void stop(int thread_id, bool wait_until_stop=true);
    static void stopAll(bool wait_until_stop=true);
    static void waitFor(int thread_id);
    static void waitForAll();
    static void setExitAction(bool wait_for_all=false);
    /// NOTE: This method can return nullptr. Take care of it.
    /// FIXME: This is trade-off not using boost::optional<shared_ptr<ofThread>> as for compatibility issue.
    static shared_ptr<ofThread> getThread(int thread_id);
};
