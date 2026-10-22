/*
 * A321836  a(n) = Sum_{d|n, n/d==1 mod 4} d^12 - Sum_{d|n, n/d==3 mod 4}
 *          d^12.
 * https://oeis.org/A321836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n, n/d==1 mod 4} d^12 - Sum_{d|n, n/d==3 mod 4}
 *            d^12
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 1625,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation factors n, generates all divisors, and accumulates the sum. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute x¹² by repeated multiplication. */
/* The result is stored in the same type as seq_t. */
static seq_t pow12(seq_t x) {
    seq_t r = x;
    for (int i = 1; i < 12; ++i)
        r *= x;
    return r;
}

/* Recursive DFS over the prime factor powers to enumerate all divisors of n. */
static void dfs(idx_t n,
                const uint64_t *pr, const uint64_t *ex,
                size_t idx, size_t cnt,
                idx_t cur, seq_t *sum)
{
    if (idx == cnt) {
        idx_t m = n / cur;                   /* quotient n/d */
        seq_t term = pow12((seq_t)cur);      /* d^12 */
        if (m % 4 == 1)
            *sum += term;                    /* subtract in the next case */
        else if (m % 4 == 3)
            *sum -= term;
        return;
    }

    uint64_t p = pr[idx];
    idx_t mult = 1;

    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        dfs(n, pr, ex, idx + 1, cnt, cur * mult, sum);
        mult *= p;
    }
}

/* Public function. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;                    /* undefined for n=0, return 0 */

    /* Buffer large enough for any factorization of a 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    dfs(n, primes, exps, 0, cnt, (idx_t)1, &sum);
    return sum;
}
