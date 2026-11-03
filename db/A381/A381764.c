/*
 * A381764  Nearest integer not equal to n with the same Hamming weight
 *          (number of 1 bits) as n.
 * https://oeis.org/A381764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nearest integer m ≠ n having the same Hamming weight
 *            (number of 1‑bits) as n. For even n it is the largest m < n with
 *            the same popcount (A243109); for odd n it is the smallest m > n
 *            with the same popcount (A057168)
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9533 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers for the build */
#include <stdint.h>
#include <stdbool.h>

/* --- next number with the same popcount (A057168) --- */
static seq_t next_same_popcount(seq_t n)
{
    /* lowest set bit */
    seq_t lowest = n & (~n + 1U);
    /* add it – produces ripple carry */
    seq_t ripple = n + lowest;
    /* bits changed by the ripple */
    seq_t ones = n ^ ripple;
    /* move the trailing ones (except the lowest) to the lowest positions */
    ones = (ones >> 2) / lowest;
    return ripple | ones;
}

/* --- previous number with the same popcount (A243109) --- */
static seq_t prev_same_popcount(seq_t n)
{
    /* Find the rightmost occurrence of the pattern "10"
       (bit i+1 = 1 and bit i   = 0). */
    idx_t i = 0;
    while (true) {
        seq_t low  = (n >> i)       & 1U;   /* bit i   */
        seq_t high = (n >> (i + 1)) & 1U;   /* bit i+1 */
        if (low == 0U && high == 1U) {
            /* swap them: clear bit i+1 and set bit i */
            n &= ~((seq_t)1U << (i + 1));
            n |= ((seq_t)1U << i);
            return n;
        }
        i++;
    }
}

/* --- main sequence function --- */
seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    /* even n → predecessor, odd n → successor */
    if ((m & 1U) == 0U) {
        return prev_same_popcount(m);
    } else {
        return next_same_popcount(m);
    }
}
