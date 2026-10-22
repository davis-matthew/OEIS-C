/*
 * A307706  Number of unitary divisors of n that are smaller than
 *          sqrt((sqrt(2) - 1)*n).
 * https://oeis.org/A307706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_unitary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A307706 – number of unitary divisors of n that are
 * strictly smaller than sqrt((sqrt(2) − 1)·n).
 *
 * The implementation:
 *   1. Compute the threshold L = floor( sqrt((sqrt(2)-1) * n) ).
 *   2. Factor n by trial division (n ≤ 10 000 in the test set).
 *   3. Enumerate all unitary divisors (choose either 1 or the full
 *      prime power for each distinct prime factor).
 *   4. Count those ≤ L (the condition d < sqrt((sqrt(2)-1)n)
 *      becomes d ≤ L because the square‑root is never integral).
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/count_unitary.h"

/* --------------------- helper: integer factorisation --------------------- */
static void factor_uint64(uint64_t n, uint64_t *pr, int *ex, int *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[(*cnt)] = p;
            ex[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {                /* remaining prime factor */
        pr[(*cnt)] = n;
        ex[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* --------------------------- main function ----------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* 1. threshold: floor( sqrt((sqrt(2)-1) * n) ) */
    const double SQRT2_M1 = 0.4142135623730950488016887242096980785696718753769;
    double tmp = SQRT2_M1 * (double)n;
    uint64_t limit = (uint64_t)floor(sqrt(tmp) + 1e-12);   /* small eps. */

    /* 2. factor n */
    uint64_t pr[32];
    int      ex[32];
    int      cnt;
    factor_uint64((uint64_t)n, pr, ex, &cnt);

    /* 3. enumerate unitary divisors ≤ limit */
    seq_t res = 0;
    count_unitary(pr, ex, cnt, 1ULL, limit, &res);

    return res;
}
