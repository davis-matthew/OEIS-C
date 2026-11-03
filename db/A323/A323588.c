/*
 * A323588  a(n) = Product_{k=1..n} (k^n + (n-k)^n).
 * https://oeis.org/A323588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (k^n + (n-k)^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 27 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* base term n^n */
    seq_t product = ipow((seq_t)n, (seq_t)n);

    idx_t half = n / 2;

    if (n % 2 == 0) {
        /* product over pairs k and n-k (k < half) */
        for (idx_t k = 1; k < half; ++k) {
            seq_t ak = ipow((seq_t)k,    (seq_t)n);
            seq_t bk = ipow((seq_t)(n-k),(seq_t)n);
            seq_t f  = ak + bk;
            product *= (f * f);
        }
        /* central term k = half */
        seq_t half_pow = ipow((seq_t)half, (seq_t)n);
        seq_t f = half_pow + half_pow;          /* 2 * half_pow */
        product *= f;
    } else {
        /* product over pairs (k, n-k) for k <= half */
        for (idx_t k = 1; k <= half; ++k) {
            seq_t ak = ipow((seq_t)k,    (seq_t)n);
            seq_t bk = ipow((seq_t)(n-k),(seq_t)n);
            seq_t f  = ak + bk;
            product *= (f * f);
        }
    }

    return product;
}
