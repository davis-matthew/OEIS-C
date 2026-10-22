/*
 * A306198  Multiplicative with a(p^e) = p^(e-1)*(p^2 - p - 1).
 * https://oeis.org/A306198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) is multiplicative with a(p^e) = p^(e‑1)*(p^2 - p - 1)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: compute base^exp for seq_t type. (Assumes seq_pow available.) */
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Not used (n >= 1) */
    seq_t result = 1;
    idx_t tmp = n;
    idx_t e = 0;

    /* factor 2 separately */
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        e++;
    }
    if (e > 0) {
        seq_t term = seq_pow(2, e - 1) * ((seq_t)2 * 2 - (seq_t)2 - 1);
        result *= term;
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            e++;
        }
        if (e > 0) {
            seq_t term = seq_pow(p, e - 1) *
                         (((seq_t)p * (seq_t)p) - (seq_t)p - 1);
            result *= term;
        }
    }

    /* remaining prime factor if any */
    if (tmp > 1) {
        seq_t term = (((seq_t)tmp * (seq_t)tmp) - (seq_t)tmp - 1);
        result *= term;
    }

    return result;
}
