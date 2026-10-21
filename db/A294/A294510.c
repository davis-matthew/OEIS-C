/*
 * A294510  Residues modulo 24 of curvatures in the Apollonian circle packing
 *          A042944.
 * https://oeis.org/A294510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence of residues modulo 24 for the Apollonian
 * packing with initial curvatures -1, 2, 3, 6 (A042944).
 * List: 2, 3, 6, 11, 14, 15, 18, 23, repeated with period 8. */
seq_t a(idx_t n)
{
    /* 8 distinct residues in ascending order. */
    static const seq_t residues[8] = {2, 3, 6, 11, 14, 15, 18, 23};

    /* n >= 1.  Compute (n-1) mod 8 to find the index in residues. */
    idx_t idx = (n - 1) % 8;
    return residues[idx];
}
