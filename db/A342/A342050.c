/*
 * A342050  Numbers k which have an odd number of trailing zeros in their
 *          primorial base representation A049345(k).
 * https://oeis.org/A342050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that the number of *
 *            trailing zeros in the primorial base representation of k is odd.
 *            * (The primorial base representation uses weights that are
 *            products * of successive primes: weight i = p1*p2*...*pi; each
 *            digit a_i satisfies * 0 ≤ a_i < p_{i+1}. Trailing...
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: prime array sufficient for the range of interest. */
static const uint64_t _primes[20] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
    53ULL, 59ULL, 61ULL, 67ULL, 71ULL
};
static const size_t _primes_cnt = 20;

seq_t a(idx_t n) {
    if (n == 0) {
        /* Undefined for n==0; return 0 as a safe placeholder. */
        return (seq_t)0;
    }

    idx_t found = 0;
    idx_t k = 2;                      /* smallest qualifying integer */
    for (;;) {
        /* Count the trailing zeros in the primorial base of k. */
        idx_t remaining = k;
        size_t pos = 0;
        size_t zero_cnt = 0;

        while (pos < _primes_cnt) {
            uint64_t prime = _primes[pos];
            uint64_t digit = (uint64_t)(remaining % prime);

            if (digit != 0U) {
                break;                  /* first non‑zero digit */
            }

            zero_cnt++;
            remaining /= prime;
            pos++;
        }

        if (zero_cnt & 1U) {            /* odd number of trailing zeros */
            found++;
            if (found == n) {
                return (seq_t)k;
            }
        }

        k += 2;                         /* next even integer */
    }
}
