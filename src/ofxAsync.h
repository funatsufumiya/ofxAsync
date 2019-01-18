#pragma once

#include "ofMain.h"

#include "boost/optional.hpp"

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
    
    template<typename T>
    class AsyncRunnerWithArgAndParam: public ofThread {
    public:
        void setup(std::function<void(T*, ofThread*)> _func, T&& _param){
            this->func = _func;
            this->param = std::move(_param);
        }
        
        void setup(std::function<void(T*, ofThread*)> _func){
            this->func = _func;
        }
        
        void threadedFunction(){
            this->func(&(this->param), this);
        }
        
        std::function<void(const T&,ofThread*)> func;
        T param;
    };
    
    static std::map<int, shared_ptr<ofThread> > runners;
    static AsyncExit asyncExit;
    static int thread_id_max;
    
public:
    static int run(std::function<void()> func);
    static int run(std::function<void(ofThread*)> func);
    template<typename T>
    static int run(std::function<void(T*, ofThread*)> func);
    template<typename T>
    static int run(std::function<void(T*, ofThread*)> func, T&& parameter);
    static void update();
    static bool exists(int thread_id);
    static bool isRunning(int thread_id);
    template<typename T>
    static boost::optional<const T&> getParameter(int thread_id);
    static void stop(int thread_id, bool wait_until_stop=true);
    static void stopAll(bool wait_until_stop=true);
    static void waitFor(int thread_id);
    static void waitForAll();
    static boost::optional<shared_ptr<ofThread>> getThread(int thread_id);
};
