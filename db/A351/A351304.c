/*
 * A351304  a(n) = n^9 * Product_{p|n, p prime} (1 + 1/p^9).
 * https://oeis.org/A351304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^9 * Product_{p|n, p prime} (1 + 1/p^9)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative: a(p^e) = p^(9e) + p^(9e-9) */
#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle n == 0 (not part of the OEIS offset) */
    if (n == 0) return (seq_t)0;

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            size_t exp = 0;
            while (m % p == 0) { m /= p; ++exp; }

            seq_t p_seq = (seq_t)p;
            seq_t p_pow9e = big_pow(p_seq, (seq_t)(9 * exp));
            seq_t term = p_pow9e + p_pow9e / big_pow(p_seq, (seq_t)9);
            result *= term;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1) {
        seq_t p_seq = (seq_t)m;
        seq_t p_pow9e = big_pow(p_seq, (seq_t)9);   /* p^(9*1) */
        seq_t term = p_pow9e + (seq_t)1;             /* p^9 + 1 */
        result *= term;
    }

    return result;
}
