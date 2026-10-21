/*
 * A276840  Number of permutations of [n] such that for each cycle c the
 *          smallest integer interval containing all elements of c has at most
 *          six elements.
 * https://oeis.org/A276840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A276840
    Formula: Number of permutations of [n] such that for each cycle c
    the smallest integer interval containing all elements of c has at most
    six elements.
    This implementation uses a transfer‑matrix DP over a 6‑bit mask that
    tracks which of the next six positions are already used by earlier
    cycles.  For each new number i we either skip it (if it is already
    marked) or we create a new cycle containing i and an arbitrary
    subset of the following unused positions.  The number of cycles
    on k+1 elements is k!; hence the recurrence uses factorials up to 5.
    Complexity: O(n · 2^5) time, O(2^5) memory. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑computed factorials up to 5!: t! for t=0..5 */
    const seq_t fact[6] = { 1, 1, 2, 6, 24, 120 };

    /* DP arrays: we only keep the values for the current position.
       dp[mask]  holds the number of ways for the current position i
       with the 6‑bit mask describing which of i..i+5 are already used. */
    seq_t dp[64] = {0};
    seq_t ndp[64] = {0};

    /* Initial mask for i = 1.  Bits 0..5 correspond to positions 1..6.
       Positions beyond n are marked as already used (bit = 1). */
    unsigned init_mask = 0;
    for (unsigned offset = 0; offset < 6; ++offset) {
        if (offset + 1 > n)
            init_mask |= (1u << offset);
    }
    dp[init_mask] = 1;

    /* Main loop over positions 1..n */
    for (idx_t i = 1; i <= n; ++i) {
        /* Zero next DP array */
        for (int m = 0; m < 64; ++m) ndp[m] = 0;

        /* Process all masks */
        for (unsigned mask = 0; mask < 64; ++mask) {
            seq_t ways = dp[mask];
            if (!ways) continue;

            /* If current position i is already used, just shift the mask. */
            if (mask & 1u) {
                unsigned next = mask >> 1u;
                if (i + 6 > n)            /* new position i+6 is outside bounds */
                    next |= 1u << 5u;     /* mark it as used */
                ndp[next] += ways;
                continue;
            }

            /* Current position i is free.  Choose a subset of the next
               unused positions (offset 1..5) to join the cycle. */
            unsigned avail = (~mask) & 0x3Eu;     /* positions 1..5 that are free */

            /* Enumerate all subsets of avail (including the empty subset). */
            unsigned sub = avail;
            while (1) {
                unsigned tilted = mask | 1u | sub;      /* mark chosen positions as used */
                unsigned next = tilted >> 1u;           /* shift window */
                if (i + 6 > n) next |= 1u << 5u;       /* mark i+6 as used if out of bounds */

                seq_t add = ways * fact[__builtin_popcount(sub)];
                ndp[next] += add;

                if (sub == 0) break;                   /* finished all subsets */
                sub = (sub - 1) & avail;               /* next subset */
            }
        }

        /* Prepare for next iteration */
        for (int m = 0; m < 64; ++m) dp[m] = ndp[m];
    }

    /* After processing n the mask with all bits set indicates that
       all positions up to n have been used. */
    return dp[63];
}
