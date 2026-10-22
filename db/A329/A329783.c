/*
 * A329783  Numbers that are either +-2 (mod 5) or +-11 (mod 55).
 * https://oeis.org/A329783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 
 * Formula: a(n) = 55 * floor((n-1)/24) + r[n]
 * where r[0..23] is the sorted list of residues modulo 55 that are
 * ±2 (mod 5) or ±11 (mod 55).
 *
 * R = {2,3,7,8,11,12,13,17,18,22,23,27,28,32,33,37,38,42,43,44,47,48,52,53}
 */
static const int residues[24] = {
    2,  3,  7,  8, 11, 12, 13, 17, 18, 22,
    23, 27, 28, 32, 33, 37, 38, 42, 43, 44,
    47, 48, 52, 53
};

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n = 0, return 0 as a safe fallback */
        return (seq_t)0;

    idx_t q = (n - 1) / 24;  /* number of complete 55‑blocks before the nth term   */
    idx_t r = (n - 1) % 24;  /* index into the residues array for the remainder   */

    /* Ensure the multiplication is performed in seq_t to handle large results.   */
    return (seq_t)q * (seq_t)55 + (idx_t)residues[r];
}
