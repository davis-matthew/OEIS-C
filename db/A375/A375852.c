/*
 * A375852  Numbers congruent to {0, 1, 3, 6, 7, 9, 12, 15} mod 18.
 * https://oeis.org/A375852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18 * ⌊(n‑1)/8⌋ + residues[(n‑1)%8]
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where residues = {0,1,3,6,7,9,12,15} */

/* Returns the n-th term of OEIS sequence A375852.  */
seq_t a(idx_t n)
{
    /* n is 1‑based (offset 1) */
    idx_t k = (n - 1) / 8;        /* full cycles of 18 before the n‑th term */
    idx_t r = (n - 1) % 8;        /* position within the cycle (0..7) */

    /* residues that are acceptable modulo 18 */
    static const seq_t residues[8] = {0, 1, 3, 6, 7, 9, 12, 15};

    /* Compute in seq_t to avoid overflow for large seq_t types. */
    return (seq_t)18 * (seq_t)k + residues[r];
}
