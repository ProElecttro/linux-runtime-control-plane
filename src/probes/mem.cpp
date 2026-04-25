#include <vector>
#include <unistd.h>

int main() {
    while (true) {
        std::vector<int> v(100000000); // ~400MB
        sleep(1);
    }
}
