/*
 * A285936  Number of rotations of the digits in n produced by n * x/y where x
 *          != y.
 * https://oeis.org/A285936
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct rotations of the decimal digits of n
 *            (excluding the trivial rotation and those that start with a
 *            zero) that share a nontrivial common divisor with n
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/digit_count.h"

/* Precomputed powers of 10 up to 10^19 (fits in uint64_t).  */
static const uint64_t POW10[20] = {
    1ULL,          10ULL,          100ULL,          1000ULL,
    10000ULL,      100000ULL,      1000000ULL,      10000000ULL,
    100000000ULL,  1000000000ULL,  10000000000ULL,  100000000000ULL,
    1000000000000ULL, 10000000000000ULL, 100000000000000ULL,
    1000000000000000ULL, 10000000000000000ULL,
    100000000000000000ULL, 1000000000000000000ULL
};

seq_t a(idx_t n)
{
    if (n < 10)   /* sequence offset starts at 10 */
        return 0;

    uint64_t d = digit_count(n);           /* number of decimal digits */
    seq_t count = 0;

    for (uint64_t k = 1; k < d; ++k) {
        uint64_t low  = n % POW10[k];
        uint64_t high = n / POW10[k];

        /* rotate: move last k digits to the front */
        __uint128_t temp = (__uint128_t)low * POW10[d - k] + high;
        uint64_t m = (uint64_t)temp;

        if (m == n)                 /* exclude the unchanged rotation */
            continue;
        if (m < POW10[d - 1])      /* exclude rotations with leading zero */
            continue;

        seq_t g = gcd((seq_t)n, (seq_t)m);
        if (g > (seq_t)1)
            count++;
    }
    return count;
}
