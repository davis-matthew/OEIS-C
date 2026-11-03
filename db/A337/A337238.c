/*
 * A337238  Number k such that k and k+1 are both digitally balanced numbers
 *          in base 2 (A031443).
 * https://oeis.org/A337238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*A031443(n) + 1
 * Range:     u8 n <= 14, u16 n <= 2353, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9631 (2026-11-01)
 * Helpers:   binomial_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A031443(n) is the n‑th digitally balanced binary number */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial_idx.h"

/* Find the n-th digitally balanced binary number (1‑based index) */
static seq_t _nth_balanced(idx_t n)
{
    idx_t rem = n;          /* 1‑based position within current length block */
    idx_t m   = 1;          /* the half‑length: total bits = 2*m */

    while (1) {
        /* Count of balanced numbers with 2*m bits */
        idx_t cnt = binomial_idx(2 * m - 1, m - 1);
        if (rem > cnt) {
            rem -= cnt;
            ++m;
            continue;
        }

        /* Construct the rem‑th number of this length */
        seq_t num = ((seq_t)1) << (2 * m - 1); /* leading 1 */
        idx_t ones_needed = m - 1;
        idx_t r = rem - 1;                    /* 0‑based inside this block */

        for (idx_t pos = 2 * m - 2; pos + 1 > 0; --pos) {
            if (ones_needed == 0) break;
            idx_t pos_bits  = pos;
            idx_t comb_zero = binomial_idx(pos_bits, ones_needed);
            if (r >= comb_zero) {
                num |= ((seq_t)1) << pos;
                r -= comb_zero;
                --ones_needed;
            }
        }
        return num;
    }
}

/* OEIS sequence a(n) */
seq_t a(idx_t n)
{
    seq_t balanced = _nth_balanced(n);
    return (seq_t)4 * balanced + (seq_t)1;
}
