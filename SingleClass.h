//
// Created by 安凌翔 on 2024/3/28.
//
#pragma once
#include <iostream>
#include <mutex>

// 线程安全的懒汉模式，在需要对象时调用get_instance获得对象
class SingleClass{
public:

    static SingleClass* get_instance(){
        if(m_instance == nullptr){
            m_mtx.lock(); // 此处要加锁，防止多个线程同时进入第一个if后，实例化出多个对象。
            if(m_instance == nullptr) m_instance = new SingleClass();
            m_mtx.unlock();
        }
        return m_instance;
    }

private:

    static std::mutex m_mtx;
    static SingleClass* m_instance;
    SingleClass();
    ~SingleClass(); // 析构函数设置为private防止实例被某一个进程析构
};


std::mutex SingleClass::m_mtx;
SingleClass* SingleClass::m_instance = nullptr;


// 懒汉单例模式，在程序开始执行时就实例化对象
class lazySingle{
public:
    static lazySingle* get_instance(){
        return m_instance;
    }

private:
    static lazySingle* m_instance;
};

lazySingle* lazySingle::m_instance = new lazySingle();