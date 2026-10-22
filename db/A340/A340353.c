/*
 * A340353  a(n) is the least nonnegative integer value of n^(n+1)/k -
 *          (n+1)^n.
 * https://oeis.org/A340353
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n+1)/k – (n+1)^n, where k is the largest divisor of
 *            n^(n+1) that satisfies n^(n+1)/k > (n+1)^n. The algorithm finds
 *            k = max{ d | d divides n^(n+1) and d ≤ floor(n^(n+1)/(n+1)^n) }.
 *            It then returns n^(n+1)/k - (n+1)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

static void dfs_divs(int idx,
                     uint64_t cur,
                     const uint64_t *p,
                     const int *maxExp,
                     int primeCnt,
                     uint64_t limit,
                     uint64_t *best)
{
    if (idx == primeCnt) {
        if (cur > *best) *best = cur;
        return;
    }
    /* use exponent 0 for this prime */
    dfs_divs(idx + 1, cur, p, maxExp, primeCnt, limit, best);
    /* try higher exponents until product would exceed limit */
    for (int e = 1; e <= maxExp[idx]; ++e) {
        if (cur > limit / p[idx]) break;      /* avoid overflow */
        cur *= p[idx];
        dfs_divs(idx + 1, cur, p, maxExp, primeCnt, limit, best);
    }
}

seq_t a(idx_t n)
{
    /* 1. Factor n */
    uint64_t n_val = (uint64_t)n;
    uint64_t primes[20];
    int  apowers[20];
    int  maxExp[20];
    int  primeCnt = 0;

    for (uint64_t p = 2; p * p <= n_val; ++p) {
        if (n_val % p == 0) {
            int a = 0;
            while (n_val % p == 0) {
                n_val /= p;
                ++a;
            }
            primes[primeCnt]   = p;
            apowers[primeCnt]  = a;
            maxExp[primeCnt]   = (int)((n + 1) * a);   /* exponent in n^(n+1) */
            ++primeCnt;
        }
    }
    if (n_val > 1) {                /* remaining prime factor */
        primes[primeCnt]   = n_val;
        apowers[primeCnt]  = 1;
        maxExp[primeCnt]   = (int)(n + 1);          /* (n+1)*1 */
        ++primeCnt;
    }

    /* 2. Compute N = n^(n+1) and A = (n+1)^n */
    seq_t N = big_pow((seq_t)n, (seq_t)(n + 1));
    seq_t A = big_pow((seq_t)(n + 1), (seq_t)n);

    /* 3. Compute t = floor(N/A) */
    seq_t t_seq = N / A;                     /* integer division is floor for positives */
    uint64_t t = (uint64_t)t_seq;            /* t is small (≈n/e) and fits in 64‑bit */

    /* 4. Find largest divisor d of N that is ≤ t */
    uint64_t best = 1;
    dfs_divs(0, 1, primes, maxExp, primeCnt, t, &best);
    seq_t d = (seq_t)best;

    /* 5. Result = N/d – A */
    seq_t res = N / d - A;
    return res;
}
