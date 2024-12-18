#include "ofxAsync.h"

std::map<int, shared_ptr<ofThread> > ofxAsync::runners = {};
ofxAsync::AsyncExit ofxAsync::asyncExit = ofxAsync::AsyncExit();
int ofxAsync::thread_id_max = 0;
bool ofxAsync::wait_for_all_when_exit = true;

int ofxAsync::run(std::function<void()> func){
    gc();

    int runner_id = ++thread_id_max;
    auto runner = make_shared<ofxAsync::AsyncRunner>();
    runners[runner_id] = runner;
    runner->setup(func);
    runner->startThread();
    return runner_id;
}

int ofxAsync::run(std::function<void(ofThread*)> func){
    gc();

    int runner_id = ++thread_id_max;
    auto runner = make_shared<ofxAsync::AsyncRunnerWithArg>();
    runners[runner_id] = runner;
    runner->setup(func);
    runner->startThread();
    return runner_id;
}

void ofxAsync::gc(){
    auto iter = runners.begin();
    auto endIter = runners.end();

    for(; iter != endIter; ) {
        if(!iter->second || !iter->second->isThreadRunning()){
            iter = runners.erase(iter);
        }else{
            ++iter;
        }
    }
}

void ofxAsync::update(){
    // do nothing, exists just for compatibility
}

bool ofxAsync::exists(int thread_id){
    return runners.count(thread_id) > 0 && runners[thread_id]->isThreadRunning();
}

bool ofxAsync::isRunning(int thread_id){
    return runners.count(thread_id) > 0 && runners[thread_id]->isThreadRunning();
}

void ofxAsync::stop(int thread_id, bool wait_until_stop){
    if(runners.count(thread_id) > 0 && runners[thread_id]->isThreadRunning()){
        auto e = runners.at(thread_id);
        e->stopThread();
        if(wait_until_stop){
            e->waitForThread(false);
        }
    }

    runners.erase(thread_id);
    gc();
}

void ofxAsync::stopAll(bool wait_until_stop){
    for(auto it = runners.begin(); it != runners.end(); ++it) {
        auto& key = it->first;
        auto e = runners.at(key);
        if(e->isThreadRunning()){
            e->stopThread();
            
            if(wait_until_stop){
                e->waitForThread(false);
            }
        }
    }

    runners.clear();
}

void ofxAsync::waitFor(int thread_id){
    if(runners.count(thread_id) > 0 && runners[thread_id]->isThreadRunning()){
        auto e = runners.at(thread_id);
        e->waitForThread(false);
    }

    runners.erase(thread_id);
    gc();
}

void ofxAsync::waitForAll(){
    for(auto it = runners.begin(); it != runners.end(); ++it) {
        auto& key = it->first;
        auto e = runners.at(key);
        if(e->isThreadRunning()){
            e->waitForThread(false);
        }
    }

    runners.clear();
}

shared_ptr<ofThread> ofxAsync::getThread(int thread_id){
    if(runners.count(thread_id) > 0 && runners[thread_id]->isThreadRunning()){
        auto e = runners.at(thread_id);
        return e;
    }else{
        return nullptr;
    }
}

void ofxAsync::setExitAction(bool wait_for_all){
    wait_for_all_when_exit = wait_for_all;
}
