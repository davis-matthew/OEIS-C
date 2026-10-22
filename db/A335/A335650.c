/*
 * A335650  Numbers that are multiples of 2,3,5, or 7 but not multiples of the
 *          product of any combination of 2,3,5, and 7.
 * https://oeis.org/A335650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer m such that exactly one of the
 *            primes 2, 3, 5, 7 divides m
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* Undefined for n = 0, but return 0 safely */
    seq_t val = 0;
    idx_t count = 0;
    while (1) {
        ++val;                      /* candidate integer */
        int cnt = (val % 2u == 0u) +
                  (val % 3u == 0u) +
                  (val % 5u == 0u) +
                  (val % 7u == 0u);
        if (cnt == 1) {             /* qualifies */
            ++count;
            if (count == n) return val;
        }
    }
}
