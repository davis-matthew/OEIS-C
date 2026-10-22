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
 * Helpers:   next_same_popcount, prev_same_popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers for the build */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_same_popcount.h"
#include "../utils/prev_same_popcount.h"

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
