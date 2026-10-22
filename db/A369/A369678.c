/*
 * A369678  a(n) = Product_{k=0..n} (3^k + 5^(n-k)).
 * https://oeis.org/A369678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (3^k + 5^(n-k))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"   /* provides: seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t pow3   = 1;                     /* 3^k */
    seq_t pow5   = big_pow((seq_t)5, (seq_t)n);   /* 5^n */

    for (idx_t k = 0; k <= n; ++k) {
        result *= (pow3 + pow5);
        pow3   *= 3;                      /* next 3^(k+1) */
        if (k < n)
            pow5 /= 5;                    /* next 5^(n-k-1) */
    }
    return result;
}
