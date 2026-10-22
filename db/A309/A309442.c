/*
 * A309442  Minimum number of colors needed to color the cells of the six
 *          regular convex polychora such that no two cells with a common face
 *          share the same color (in the order 5-cell, 8-cell, 16-cell,
 *          24-cell, 120-cell, 600-cell).
 * https://oeis.org/A309442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: minimal number of colors needed to color the cells of the 6
   regular convex 4-polytopes.  Values are stored in a small lookup array. */

seq_t a(idx_t n)
{
    /* Return the corresponding table entry for n = 1..6.
       For other n, return 0 as the sequence is only defined for the
       six regular polychora. */
    switch (n) {
        case 1: return (seq_t)5;  /* 5-cell */
        case 2: return (seq_t)4;  /* 8-cell */
        case 3: return (seq_t)2;  /* 16-cell */
        case 4: return (seq_t)3;  /* 24-cell */
        case 5: return (seq_t)5;  /* 120-cell */
        case 6: return (seq_t)3;  /* 600-cell */
        default: return (seq_t)0; /* undefined beyond n=6 */
    }
}
