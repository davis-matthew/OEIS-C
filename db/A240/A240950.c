/*
 * A240950  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period, omitting those values of k of the form m^2+1.
 * https://oeis.org/A240950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A240950
 *
 * The sequence contains numbers k for which the continued‑fraction
 * period of √k is odd, with the exclusion of k = m² + 1.  The
 * implementation performs a brute‑force search: starting from k = 2
 * it tests candidates in ascending order and counts those that satisfy
 * the conditions until the nth term is found.
 *
 * This pure C code works for all supported seq_t/idx_t types,
 * including arbitrary‑precision seq_t classes used in the C++ build.
 * No dynamic allocation is required.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer square root of a 64‑bit unsigned integer (largest r with r*r <= x) */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* length of the continued‑fraction period of √k, where k is not a square */
static uint64_t cf_period_length(uint64_t k)
{
    uint64_t a0 = isqrt_u64(k);
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;
    uint64_t period = 0;

    while (true) {
        m = d * a - m;
        d = (k - m * m) / d;
        a = (a0 + m) / d;
        ++period;
        if (a == 2 * a0)
            break;
    }
    return period;
}

/* External linkage function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n == 0 */

    uint64_t count = 0;            /* number of valid terms found */
    uint64_t cand  = 2;            /* current candidate k */

    while (count < n) {
        uint64_t root = isqrt_u64(cand);

        /* skip perfect squares */
        if (root * root == cand) {
            ++cand;
            continue;
        }

        /* skip numbers of the form m^2 + 1 */
        if (root * root + 1ULL == cand) {
            ++cand;
            continue;
        }

        /* check if the continued‑fraction period is odd */
        if ((cf_period_length(cand) & 1ULL) != 0) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }

        ++cand;
    }

    /* should never reach here */
    return 0;
}
