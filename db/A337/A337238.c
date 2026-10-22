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
 * Helpers:   _comb_idx, _nth_balanced
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A031443(n) is the n‑th digitally balanced binary number */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_comb_idx.h"
#include "../utils/_nth_balanced.h"

/* OEIS sequence a(n) */
seq_t a(idx_t n)
{
    seq_t balanced = _nth_balanced(n);
    return (seq_t)4 * balanced + (seq_t)1;
}
