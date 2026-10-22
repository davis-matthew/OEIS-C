/*
 * A369189  Sum of the squarefree triangular divisors of n.
 * https://oeis.org/A369189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} d * mu(d)^2 * c(d) * where c(d) = 1 if d is a
 *            triangular number, 0 otherwise * (i.e. 8*d+1 is a perfect
 *            square)
 * Range:     u8 n <= 9, u16 n <= 1041, u32 n <= 9137, u64 n <= 9137,
 *            u128 n <= 9137, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

/* Recursive helper to enumerate all divisors of n
 * while keeping track of whether the divisor is squarefree. */
static void dfs(const uint64_t *primes, const uint64_t *exps,
                size_t cnt, size_t idx,
                seq_t cur, bool bad,
                seq_t *acc) {
    if (idx == cnt) {
        if (!bad) {
            seq_t val = (seq_t)8 * cur + (seq_t)1;
            seq_t r = floor_sqrt(val);
            if (r * r == val) {
                *acc += cur;
            }
        }
        return;
    }

    uint64_t p = primes[idx];
    uint64_t maxe = exps[idx];
    seq_t mul = (seq_t)1;
    for (uint64_t e = 0; e <= maxe; ++e) {
        bool nextBad = bad || (e >= 2);
        dfs(primes, exps, cnt, idx + 1, cur * mul, nextBad, acc);
        mul *= (seq_t)p;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    /* Factorise n (simple trial division, n ≤ 10000 is trivial). */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            primes[cnt] = (uint64_t)p;
            exps[cnt] = 0;
            while (m % p == 0) {
                ++exps[cnt];
                m /= p;
            }
            ++cnt;
        }
    }
    if (m > 1) {
        primes[cnt] = (uint64_t)m;
        exps[cnt] = 1;
        ++cnt;
    }

    seq_t sum = (seq_t)0;
    dfs(primes, exps, cnt, 0, (seq_t)1, false, &sum);
    return sum;
}
