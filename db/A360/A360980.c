/*
 * A360980  a(n) is the least multiple of n that is an odious number
 *          (A000069).
 * https://oeis.org/A360980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for the least multiple of n that is odious */
/*   a(n) = min_{k ≥ 1} n·k such that n·k has an odd number of 1‑bits */
#include <stdbool.h>

/* Return true iff x has an odd popcount (odious number). */
static bool is_odious(seq_t x)
{
    seq_t parity = 0;
    while (x != 0) {
        parity ^= x & 1;   /* toggle parity for each 1 bit */
        x >>= 1;           /* shift right to inspect next bit */
    }
    return (parity & 1) != 0;
}

seq_t a(idx_t n)
{
    /* Convert n to seq_t once to minimize casts. */
    seq_t n_seq = (seq_t)n;

    /* If n itself is odious, it is the minimal multiple. */
    if (is_odious(n_seq))
        return n_seq;

    /* Search for the smallest multiplier k > 1. */
    for (idx_t k = 2; ; ++k) {
        seq_t prod = n_seq * ((seq_t)k);
        if (is_odious(prod))
            return prod;
    }
}
