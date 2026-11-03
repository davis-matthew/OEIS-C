/*
 * A360271  a(n) = Sum_{k=0..floor(n/4)} (-1)^k * binomial(n-3*k,k) *
 *          Catalan(n-3*k).
 * https://oeis.org/A360271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} (-1)^k * binomial(n-3k, k) *
 *            Catalan(n-3k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 4;
    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t idx = n - 3 * k;
        seq_t bin = binomial(idx, k);
        seq_t cat = catalan(idx);
        seq_t term = bin * cat;
        if (k & 1)
            neg += term;
        else
            pos += term;
    }
    return pos - neg;   /* pos ≥ neg by construction */
}
