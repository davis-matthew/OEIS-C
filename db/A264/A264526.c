/*
 * A264526  Smallest number m such that both 2*n-m and 2*n+m are primes.
 * https://oeis.org/A264526
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer m such that 2*n - m and 2*n + m
 *            are primes. Sequence A264526
 * Range:     u8 n <= 4251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;           /* below offset, behavior not required */
    idx_t center = 2 * n;          /* 2*n fits comfortably into idx_t for n <= 10000 */

    for (idx_t m = 1; ; m += 2) {  /* m is odd – even m would give both numbers even */
        idx_t low  = center - m;   /* 2*n - m */
        if (low < 2) break;        /* below 2 no primes can occur, stop search */
        idx_t high = center + m;   /* 2*n + m */

        if (is_prime(low) && is_prime(high)) {
            return (seq_t)m;       /* found smallest m */
        }
    }

    return 0; /* should not happen for n >= 2 under the definition */
}
