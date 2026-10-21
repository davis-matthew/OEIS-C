/*
 * A280505  The palindromic kernel of n in base 2 (with carryless GF(2)[X]
 *          factorization): a(n) = A091255(n,A057889(n)).
 * https://oeis.org/A280505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the maximal GF(2)[X]-divisor of n * whose binary
 *            representation is a palindrome * (or becomes one after removing
 *            trailing 0's)
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Checks whether denom divides num in GF(2)[X] (carry‑less polynomial division). */
static bool divides(seq_t num, seq_t denom)
{
    if (denom == 0) return false;            /* should never happen */
    seq_t rem = num;
    const idx_t ddeg = bitlen_seq_t(denom);

    while (rem != 0) {
        idx_t rdeg = bitlen_seq_t(rem);
        if (rdeg < ddeg) break;
        idx_t shift = rdeg - ddeg;
        rem ^= denom << shift;
    }
    return rem == 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* not used, but harmless */

    seq_t num = (seq_t)n;
    idx_t bits_n = bitlen_seq_t(num);        /* highest set bit position +1 */

    seq_t best = 1;                           /* minimal kernel is 1 */

    /* Enumerate all palindrome polynomials of length len (1..bits_n). */
    for (idx_t len = 1; len <= bits_n; ++len) {
        idx_t half = (len - 1) / 2;           /* independent bits (excluding endpoints) */
        idx_t maxMask = 1ULL << half;        /* number of combinations for interior bits */

        for (idx_t mask = 0; mask < maxMask; ++mask) {
            /* Construct palindrome with trailing 1. */
            seq_t pal = ((seq_t)1 << (len - 1)) | 1;   /* set highest and lowest bits */

            /* Set interior bits according to mask. */
            for (idx_t i = 0; i < half; ++i) {
                if ((mask >> i) & 1ULL) {
                    idx_t pos_low  = i + 1;
                    idx_t pos_high = len - 2 - i;
                    pal |= ((seq_t)1 << pos_low);
                    pal |= ((seq_t)1 << pos_high);
                }
            }

            /* Add trailing zeros (multiply by X^shift). */
            idx_t maxShift = bits_n > len ? bits_n - len : 0;
            for (idx_t shift = 0; shift <= maxShift; ++shift) {
                seq_t cand = pal << shift;
                if (cand > num) continue;      /* too large to divide */
                if (divides(num, cand) && cand > best) best = cand;
            }
        }
    }
    return best;
}
