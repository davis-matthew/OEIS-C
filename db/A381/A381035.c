/*
 * A381035  Numbers such that the least significant nonzero digit in their
 *          primorial base representation (A049345) is 1.
 * https://oeis.org/A381035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer whose least significant non‑zero digit
 *            in its primorial base representation equals 1
 * Range:     u8 n <= 183, u16 n <= 9305, u32 n <= 9305, u64 n <= 9305,
 *            u128 n <= 9305, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>   /* size_t */

/* Small list of primes – enough for the values reached when n ≤ 10000. */
#define PRIME_COUNT 25
static const uint64_t PRIME_LIST[PRIME_COUNT] = {
    2ULL,   3ULL,   5ULL,   7ULL,  11ULL,  13ULL,  17ULL,  19ULL,  23ULL,
   29ULL,  31ULL,  37ULL,  41ULL,  43ULL,  47ULL,  53ULL,  59ULL,  61ULL,
   67ULL,  71ULL,  73ULL,  79ULL,  83ULL,  89ULL,  97ULL
};

/* Return true if the least significant non‑zero digit in the
   primorial base representation of x is 1. */
static bool least_nonzero_digit_is_one(seq_t x)
{
    seq_t m = x;
    for (size_t i = 0; i < PRIME_COUNT; ++i) {
        seq_t p = (seq_t)PRIME_LIST[i];
        seq_t d = m % p;
        m = m / p;
        if (d != (seq_t)0)
            return d == (seq_t)1;
        if (m == (seq_t)0)
            break;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) abort();          /* sequence is 1‑based with offset 1 */
    idx_t    count = 0;
    idx_t    cand   = 1;

    while (1) {
        if (least_nonzero_digit_is_one((seq_t)cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}
