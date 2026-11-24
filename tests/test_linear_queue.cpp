// Unit tests for src/linear_queue.cpp
// The program's own main() is renamed to demo_main() so it can be tested too.
#include <iostream>
#include <sstream>
#include <string>

#define main demo_main
#include "../src/linear_queue.cpp"
#undef main

static int failures = 0;
static int checks = 0;

#define CHECK(cond, name)                                            \
    do {                                                             \
        ++checks;                                                    \
        if (cond) std::cout << "  PASS  " << name << "\n";           \
        else { ++failures; std::cout << "  FAIL  " << name << "\n"; } \
    } while (0)

// Runs f() while capturing everything written to std::cout.
template <typename F>
static std::string capture(F f) {
    std::ostringstream buf;
    std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
    f();
    std::cout.rdbuf(old);
    return buf.str();
}

int main() {
    std::cout << "linear_queue tests\n";

    // 1. The bundled demo prints exactly what the README documents.
    CHECK(capture([] { demo_main(); }) == "30\n50\n90\nqueue is empty\n0\n",
          "demo output: 30 50 90, then 'queue is empty' and 0");

    // 2. FIFO order.
    {
        queue q;
        q.addQ(1); q.addQ(2); q.addQ(3);
        bool ok = q.delQ() == 1 && q.delQ() == 2 && q.delQ() == 3;
        CHECK(ok, "items leave in the order they entered (FIFO)");
    }

    // 3. Underflow on a brand-new queue.
    {
        queue q;
        int v = -1;
        std::string out = capture([&] { v = q.delQ(); });
        CHECK(out == "queue is empty\n" && v == 0, "dequeue from empty queue -> message and 0");
    }

    // 4. Overflow: capacity is QUEUE_CAPACITY (5); the 6th item is rejected.
    {
        queue q;
        std::string out = capture([&] { for (int i = 1; i <= 6; ++i) q.addQ(i * 10); });
        CHECK(out == "queue is full\n", "6th enqueue prints 'queue is full'");
        bool ok = true;
        for (int i = 1; i <= 5; ++i) ok = ok && q.delQ() == i * 10;
        CHECK(ok, "the five accepted items are intact; the rejected one is dropped");
        std::string out2 = capture([&] { q.delQ(); });
        CHECK(out2 == "queue is empty\n", "queue is empty after draining five items");
    }

    // 5. Documented limitation: space is never reused.
    {
        queue q;
        for (int i = 0; i < QUEUE_CAPACITY; ++i) q.addQ(i);
        for (int i = 0; i < QUEUE_CAPACITY; ++i) q.delQ();   // queue is logically empty again
        std::string out = capture([&] { q.addQ(99); });
        CHECK(out == "queue is full\n",
              "KNOWN LIMITATION: empty-but-exhausted queue still reports 'queue is full'");
    }

    // 6. Partial drain does not free space either.
    {
        queue q;
        for (int i = 0; i < QUEUE_CAPACITY; ++i) q.addQ(i);
        q.delQ(); q.delQ();                                   // 3 items left, 2 slots at the front are wasted
        std::string out = capture([&] { q.addQ(7); });
        CHECK(out == "queue is full\n",
              "KNOWN LIMITATION: freed front slots are not reused");
    }

    // 7. A genuinely stored 0 is indistinguishable from the 'empty' return value.
    {
        queue q;
        q.addQ(0);
        int stored = q.delQ();
        std::string out = capture([&] { q.delQ(); });
        CHECK(stored == 0 && out == "queue is empty\n",
              "KNOWN LIMITATION: a stored 0 and the empty-queue sentinel 0 look identical to the caller");
    }

    std::cout << (checks - failures) << "/" << checks << " checks passed\n";
    return failures == 0 ? 0 : 1;
}
