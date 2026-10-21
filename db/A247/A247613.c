/*
 * A247613  a(n) = Sum_{k=0..8} binomial(16,k)*binomial(n,k).
 * https://oeis.org/A247613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..8} binomial(16,k) * binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 20, u64 n <= 297, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Precomputed binomial(16,k) for k = 0..8 */
    static const seq_t bin16[9] = {
        (seq_t)1,    /* C(16,0) */
        (seq_t)16,   /* C(16,1) */
        (seq_t)120,  /* C(16,2) */
        (seq_t)560,  /* C(16,3) */
        (seq_t)1820, /* C(16,4) */
        (seq_t)4368, /* C(16,5) */
        (seq_t)8008, /* C(16,6) */
        (seq_t)11440 /* C(16,7) */
    };
    static const seq_t bin16_last = (seq_t)12870; /* C(16,8) */

    seq_t result = 0;
    for (idx_t k = 0; k < 8; ++k)
    {
        seq_t bk = bin16[k];
        seq_t bn = binomial(n, k);
        result += bk * bn;
    }
    result += bin16_last * binomial(n, 8);
    return result;
}
