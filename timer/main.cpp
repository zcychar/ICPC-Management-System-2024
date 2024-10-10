#include <chrono>
#include <iostream>

std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
long totduration;

void start() {
    start_time = std::chrono::system_clock::now();
}

void stop() {
    auto end_time = std::chrono::system_clock::now();
    totduration += std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
}

double duration() {
    return (double) totduration * std::chrono::nanoseconds::period::num / std::chrono::nanoseconds::period::den;
}

void show() {
    std::cout << " took " << duration() << " seconds\n";  // Change this line
}

const int N = 1e9;
int a[N];

int main() {
    start();
    for (int i = 0; i < N; ++i) {
        a[i] = i;
    }
    stop();
    show();
    return 0;
}
