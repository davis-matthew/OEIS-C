/*
 * A294926  Number of proper divisors of n that are deficient (A005100).
 * https://oeis.org/A294926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { d | n, d < n, sigma(d) < 2*d } (number of proper
 *            divisors of n that are deficient)
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9924 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether a positive integer n is deficient.  1 is considered deficient. */
static inline bool is_deficient(idx_t n)
{
    if (n == 1)            /* 1 has sum_of_proper_divisors = 0 < 1 */
        return true;

    unsigned __int128 sigma = 1;   /* product of (p^(e+1)-1)/(p-1) for each prime power */
    idx_t temp = n;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            unsigned __int128 part = 1;
            idx_t pe = 1;
            while (temp % p == 0) {
                temp /= p;
                pe *= p;
                part += pe;
            }
            sigma *= part;
        }
    }
    if (temp > 1)                /* remaining prime factor */
        sigma *= (1 + temp);

    unsigned __int128 proper = sigma - n;   /* sum of proper divisors */
    return proper < n;          /* deficient iff sigma < 2*n */
}

/* Compute floor(sqrt(n)) for uint64_t without overflow. */
static inline idx_t uint64_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;              /* 1 has no proper divisors */

    seq_t count = 0;
    idx_t root = uint64_sqrt(n);

    for (idx_t d = 1; d <= root; ++d) {
        if (n % d != 0)
            continue;
        idx_t d1 = d;
        idx_t d2 = n / d;

        if (d1 < n && is_deficient(d1))
            ++count;

        if (d2 != d1 && d2 < n && is_deficient(d2))
            ++count;
    }
    return count;
}
