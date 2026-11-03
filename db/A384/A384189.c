/*
 * A384189  Numbers whose number of zeros in their binary representation is
 *          not equal to 1.
 * https://oeis.org/A384189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer m such that the number of zeros
 *            in the binary representation of m is NOT equal to 1. The
 *            sequence is thus the complement of numbers with exactly one zero
 *            in binary (e.g., 2, 4, 8, 16, 32, …)
 * Range:     u8 n <= 227, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9731 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return true iff the binary representation of x contains a number
   of zeros different from 1.  x > 0 is required. */
static inline bool has_zero_count_other_than_one(idx_t x)
{
    uint64_t w = (uint64_t)x;                 /* candidate fits in 64 bits */
    unsigned ones  = __builtin_popcountll(w);
    unsigned bits  = 64 - __builtin_clzll(w);  /* number of bits used */
    unsigned zeros = bits - ones;
    return zeros != 1;
}

seq_t a(idx_t n)
{
    /* Handle the off‑by‑one: a(1) is the first term.   */
    if (n == 0)
        return (seq_t)0;                      /* undefined slot */

    idx_t candidate = 1;       /* start from 1 */
    idx_t count     = 0;       /* how many valid terms seen so far */

    while (count < n) {
        if (has_zero_count_other_than_one(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }

    /* Should never reach here, but keep compiler happy. */
    return (seq_t)0;
}
