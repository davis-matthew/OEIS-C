/*
 * A349606  Dirichlet convolution of the binary digital sum function (A000120)
 *          with itself.
 * https://oeis.org/A349606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n} A000120(d) * A000120(n/d)
 * Range:     u8 n <= 1259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive helper to enumerate all divisors of n */
static void dfs(int idx,
                uint64_t cur,
                const uint64_t *primes,
                const int *exps,
                int pcnt,
                idx_t n,
                seq_t *sum)
{
    if (idx == pcnt) {
        uint64_t d  = cur;
        uint64_t nd = n / d;
        int pc_d  = __builtin_popcountll(d);
        int pc_nd = __builtin_popcountll(nd);
        *sum += (seq_t)pc_d * (seq_t)pc_nd;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t pow = 1;
    for (int e = 0; e <= exps[idx]; ++e) {
        dfs(idx + 1, cur * pow, primes, exps, pcnt, n, sum);
        pow *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {          /* offset starts at 1, but guard for safety */
        return 0;
    }
    if (n == 1) {
        return 1;          /* a(1) = 1 */
    }

    /* Factor n using trial division */
    uint64_t primes[64];
    int exps[64];
    int pcnt = 0;
    uint64_t tmp = n;

    if ((tmp & 1ULL) == 0) {
        int e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            ++e;
        }
        primes[pcnt] = 2ULL;
        exps[pcnt] = e;
        ++pcnt;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt] = 1;
        ++pcnt;
    }

    seq_t sum = 0;
    dfs(0, 1ULL, primes, exps, pcnt, n, &sum);
    return sum;
}
