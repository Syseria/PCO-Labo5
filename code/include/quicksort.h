#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <iostream>
#include <queue>
#include <iterator>

#include <pcosynchro/pcothread.h>
#include <pcosynchro/pcomutex.h>
#include <pcosynchro/pcoconditionvariable.h>
#include "multithreadedsort.h"

/**
 * @brief The Quicksort class implements the multi-threaded Quicksort algorithm.
 */
template<typename T>
class Quicksort: public MultithreadedSort<T> {
public:
    Quicksort(unsigned int nbThreads) : MultithreadedSort<T>(nbThreads) {}

    /**
     * @brief sort Manages the threads to sort the given sequence.
     * @param array is the sequence to sort
     */
    void sort(std::vector<T>& array) override {
        std::vector<std::unique_ptr<PcoThread>> threads;

        mutex.lock();
        for (size_t i = 0; i < this->nbThreads; ++i) {
            threads.emplace_back(std::make_unique<PcoThread>(&Quicksort::worker, this));
        }

        ++activeThread;
        tasks.emplace([this, &array]() {
            quicksort(array.begin(), array.end());
            finishTask();
        });
        cond.notifyOne();
        mutex.unlock();

        for (auto& thread : threads) {
            thread->join();
        }
    }

private:

    void worker() {
        while (true) {
            std::function<void()> task;

            mutex.lock();
            cond.wait(&mutex);

            if (tasks.empty() && activeThread == 0) {
                break;
            }

            task = tasks.front();
            tasks.pop();

            mutex.unlock();

            task();
        }
    }

    /**
     * @brief quicksort Sorts in place a sequence from a vector dilimited by lo and hi.
     * @param lo        iterator on the begin of your sequence
     * @param hi        iterator on the end of your sequence
     * @param depth     depth or current iteration, used for multithreading
     */
    void quicksort (typename std::vector<T>::iterator lo, typename std::vector<T>::iterator hi) {
        if (lo != hi && std::distance(lo, hi) > 1) {
            typename std::vector<T>::iterator pivot = partition(lo, hi);

            mutex.lock();

            if (activeThread < this->nbThreads) {
                ++activeThread;
                tasks.emplace([this, lo, pivot]() {
                    quicksort(lo, pivot);
                    finishTask();
                });
                cond.notifyOne();
            } else {
                quicksort(lo,pivot);
            }

            quicksort(std::next(pivot), hi);
        }
    }

    void finishTask() {
        mutexFinished.lock();
        --activeThread;
        cond.notifyAll();
        mutexFinished.unlock();
    }

    /**
     * @brief partition Partion the vector to use the returned iterator as pivot for the quicksort() algorithm.
     * @param lo        iterator on the begin of your sequence
     * @param hi        iterator on the end of your sequence
     * @return          a vector::iterator to the pivot
     */
    typename std::vector<T>::iterator partition (typename std::vector<T>::iterator lo, typename std::vector<T>::iterator hi) {
        typename std::vector<T>::iterator pivot = std::prev(hi);

        typename std::vector<T>::iterator i = lo;
        for (typename std::vector<T>::iterator j = lo; j != std::prev(hi); ++j) {
            if (*j < *pivot) {
                std::iter_swap(i, j);
                ++i;
            }
        }
        std::iter_swap(i, pivot);

        return i;
    }

    PcoMutex mutex, mutexFinished;
    PcoConditionVariable cond;
    int activeThread = 0;
    std::queue<std::function<void()>> tasks;
    bool pivotFound, partitionDone;
    //unsigned int nbThreads;
};


#endif // QUICKSORT_H
