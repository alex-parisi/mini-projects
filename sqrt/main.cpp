#include <cstdio>
#include <chrono>
#include <stdexcept>
#include <cmath>
#include <math.h>


using namespace std;


typedef std::chrono::high_resolution_clock Clock;


/**
 * Square-root approximation exploiting the IEEE 754 single precision floating point format
 * Not accurate enough to be used to calculate the square-root, but useful to seed Newton's Method
 *
 * @param num Number to calculate the square root of.
 * @return Square root approximation of the input.
 */
float sqrt_approx(float num) {

    if(num < 0)
        throw invalid_argument("Error: input must be greater than or equal to zero.");

    union { float f; uint32_t i; } val = {num};

    val.i -= 1 << 23;
    val.i >>= 1;
    val.i += 1 << 29;

    return val.f;

}


/**
 * Square-root function using the math.h library
 *
 * @param num Number to calculate the square root of.
 * @return Square root of the input.
 */
float sqrt_1(float num) {

    if(num < 0)
        throw invalid_argument("Error: input must be greater than or equal to zero.");

    return pow(num, 0.5);

}


/**
 * Square-root function using the cmath library
 *
 * @param num Number to calculate the square root of.
 * @return Square root of the input.
 */
float sqrt_2(float num) {

    if(num < 0)
        throw invalid_argument("Error: input must be greater than or equal to zero.");

    return sqrt(num);

}


/**
 * Square-root function using Newton's Method
 * Initial Guess = IEEE approximation
 * Only perform two iterations - one is not precise enough
 * 
 * @param num Number to calculate the square root of.
 * @return Square root of the input.
 */
float sqrt_3(float num) {

    if(num < 0)
        throw invalid_argument("Error: input must be greater than or equal to zero.");

    float x = sqrt_approx(num); // Initial guess
    x = 0.5 * (x + (num / x)); // 1st iteration

    return 0.5 * (x + (num / x)); // 2nd iteration

}


/**
 * Function to print the results and runtime
 *
 * @param method_id Used to identify which method was used to calculate the square root.
 * @param num Input to the square root function.
 * @param result Output from the square root function.
 * @param runtime Average runtime (in nanoseconds) of the square root function.
 */
void print_result(string method, int8_t num, float result, int64_t runtime) {

    printf("\n");
    printf("%s:\n", method.c_str());
    printf("\tsqrt(%d) = %18.17f\n", num, result);
    printf("\tAverage runtime = %lu nanoseconds.\n", runtime);

}


int main(int argc, char** argv)  {

    // Initialization
    int8_t num = 42;
    int16_t test_runs = 10000;

    // Calculate sqrt (Method 1)
    {
        int64_t total_runtime_ns = 0;
        float result;
        for(int i = 0; i < test_runs; i++) {

            auto time_start = Clock::now();
            result = sqrt_1((float)num);
            auto time_stop = Clock::now();
            total_runtime_ns += chrono::duration_cast<std::chrono::nanoseconds>(time_stop - time_start).count();

        }
        // Print results
        print_result("math.h pow function", num, result, total_runtime_ns / test_runs);
    }

    // Calculate sqrt (Method 2) *** BEST ***
    {
        int64_t total_runtime_ns = 0;
        float result;
        for(int i = 0; i < test_runs; i++) {

            auto time_start = Clock::now();
            result = sqrt_2((float)num);
            auto time_stop = Clock::now();
            total_runtime_ns += chrono::duration_cast<std::chrono::nanoseconds>(time_stop - time_start).count();
            
        }
        // Print results
        print_result("cmath sqrt function", num, result, total_runtime_ns / test_runs);
    }

    // Calculate sqrt (Method 3)
    {
        int64_t total_runtime_ns = 0;
        float result;
        for(int i = 0; i < test_runs; i++) {

            auto time_start = Clock::now();
            result = sqrt_3((float)num);
            auto time_stop = Clock::now();
            total_runtime_ns += chrono::duration_cast<std::chrono::nanoseconds>(time_stop - time_start).count();
            
        }
        // Print results
        print_result("Newton's Method", num, result, total_runtime_ns / test_runs);
    }

    printf("\n");

    return 0;

}