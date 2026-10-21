/*
 * A243036  Number of entries of length n in A240602.
 * https://oeis.org/A243036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 2; For n > 1: a(n) = 2^(popcount(n) - 1) Equivalent
 *            recurrence: a(2m) = a(m) (m > 1), a(2m+1) = 2 * a(m). This
 *            implementation uses the popcount formula for O(1) time
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special cases matching the initial values in the OEIS entry. */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;

    /* Count the number of set bits in n (popcount).  n is an unsigned
       64‑bit integer (idx_t), so __builtin_popcountll is safe. */
    unsigned int pc = (unsigned int)__builtin_popcountll((unsigned long long)n);

    /* For n > 1, the value is 2^(pc-1).  The shift amount is in [0,63],
       so it fits in either 8,16,32,64 or 128‑bit seq_t. */
    return ((seq_t)1 << (pc - 1));
}
