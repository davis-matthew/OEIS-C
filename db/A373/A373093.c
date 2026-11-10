/*
 * A373093  The fixed point of the iterations of the map x -> A093653(x) that
 *          start at n.
 * https://oeis.org/A373093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = fixed point of x -> A093653(x) iterated from n, where
 *            A093653(x) = Sum_{d|x} A000120(d); computed by direct iteration
 *            until x = A093653(x)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9647 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void A373093_gen(int idx, uint64_t cur, const uint64_t *pr, const int *ex,
                        int cnt, uint64_t *sum)
{
    if (idx == cnt) {
        *sum += (uint64_t)__builtin_popcountll(cur);
        return;
    }
    uint64_t val = 1;
    for (int e = 0; e <= ex[idx]; ++e) {
        A373093_gen(idx + 1, cur * val, pr, ex, cnt, sum);
        val *= pr[idx];
    }
}

static uint64_t A373093_step(uint64_t nn)
{
    uint64_t primes[20];
    int exps[20];
    int cnt = 0;
    uint64_t tmp = nn;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            do { tmp /= p; ++e; } while (tmp % p == 0);
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {
        primes[cnt] = tmp;
        exps[cnt] = 1;
        ++cnt;
    }
    uint64_t sum = 0;
    A373093_gen(0, 1, primes, exps, cnt, &sum);
    return sum;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    for (;;) {
        uint64_t y = A373093_step(x);
        if (y == x) break;
        x = y;
    }
    return (seq_t)x;
}
