/*
 * A369720  The sum of divisors of the smallest cubefull number that is a
 *          multiple of n.
 * https://oeis.org/A369720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} ((p^(max(e,3)+1)–1)/(p–1)) (multiplicative,
 *            with e the exponent of p in n)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 1108, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Return a(n) for sequence A369720. */
seq_t a(idx_t n)
{
    /* Handle the special case n = 1.  The smallest cubefull multiple
       of 1 is 1 itself, and σ(1) = 1. */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Factor n.  For a 64‑bit integer the number of distinct prime
       factors never exceeds 16, so 64 slots are plenty. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);

    seq_t product = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        uint64_t e = ex[i];
        uint64_t k = (e >= 3) ? e : 3;        /* exponent needed in m */
        seq_t pw = big_pow(p, (seq_t)(k + 1)); /* p^(k+1) */
        seq_t numerator = pw - (seq_t)1;
        seq_t denom = p - (seq_t)1;
        seq_t sigma = numerator / denom;     /* exact division */
        product *= sigma;
    }
    return product;
}
