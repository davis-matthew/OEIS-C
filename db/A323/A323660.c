/*
 * A323660  a(n) = Product_{k=0..n} (k^11 + (n-k)^11).
 * https://oeis.org/A323660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} (k^11 + (n-k)^11)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the product in ascending order using the supplied big_pow utility
 * to efficiently raise integers to the 11th power.  All arithmetic is done
 * in the integer type seq_t, allowing arbitrary‑precision results.  For
 * the special case n = 0 the product evaluates to 0 as required. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset explicitly: a(0) = 0 */
    if (n == 0)
        return (seq_t)0;

    seq_t result = (seq_t)1;
    for (idx_t k = (idx_t)0; k <= n; ++k) {
        seq_t base1 = (seq_t)k;
        seq_t base2 = (seq_t)(n - k);

        seq_t p1 = big_pow(base1, (seq_t)11);
        seq_t p2 = big_pow(base2, (seq_t)11);

        seq_t term = p1 + p2;
        result = result * term;
    }
    return result;
}
