#include <gtest/gtest.h>

#include "quicksort.h"
#include "utils.h"

/**
 * @brief test Generates a random sequence of specified size and sorts it with Quicksort using N threads.
 * @param nbThreads number of threads to use to sort the sequence
 * @param size of the sequence to sort
 * @param seed to use for the random generation of the sequence
 */
void test(int nbThreads, int size, int seed) {
    Quicksort<int> sorter(nbThreads);
    std::vector<int> array = generateSequence(size, seed);
    sorter.sort(array);
    EXPECT_FALSE(array.empty());  // check that the result is not empty
    EXPECT_TRUE(isSorted(array)); // check that result is sorted
}

/**
 * @brief testFalse Generates a random sequence of specified size and sorts it with Quicksort using N threads.
 * @param nbThreads number of threads to use to sort the sequence
 * @param size of the sequence to sort
 * @param seed to use for the random generation of the sequence
 */
void testFalse(int nbThreads, int size, int seed) {
    Quicksort<int> sorter(nbThreads);
    std::vector<int> array = generateSequence(size, seed);
    sorter.sort(array);
    EXPECT_FALSE(array.empty());  // check that the result is not empty
    EXPECT_FALSE(isSorted(array)); // check that result is sorted
}

/**
 * @brief testFalse Generates a random sequence of specified size and sorts it with Quicksort using N threads.
 * @param nbThreads number of threads to use to sort the sequence
 * @param size of the sequence to sort
 * @param seed to use for the random generation of the sequence
 */
void testSize0(int nbThreads, int size, int seed) {
    Quicksort<int> sorter(nbThreads);
    std::vector<int> array = generateSequence(size, seed);
    sorter.sort(array);
    EXPECT_TRUE(array.empty());  // check that the result is not empty
    EXPECT_TRUE(isSorted(array)); // check that result is sorted
}

TEST(SortingTest, TestMoreThreadThanSize) {
    int size = 10;
    int nbThreads = 15;
    int seed = 0;

    test(nbThreads, size, seed);
}

TEST(SortingTest, TestSize0) {
    int size = 0;
    int nbThreads = 10;
    int seed = 0;

    testSize0(nbThreads, size, seed);
}

TEST(SortingTest, Test0Thread) {
    int size = 100;
    int nbThreads = 0;
    int seed = 0;

    testFalse(nbThreads, size, seed);
}

TEST(SortingTest, Test1Thread) {
    int size = 100;
    int nbThreads = 1;
    int seed = 0;

    test(nbThreads, size, seed);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
