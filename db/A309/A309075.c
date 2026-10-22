/*
 * A309075  Total number of black cells after n iterations of Langton's ant
 *          with two ants on the grid placed side-by-side on neighboring
 *          squares and initially looking in the same direction.
 * https://oeis.org/A309075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is periodic with period 14. Let r = n mod 14. The values
 *            for r = 0..7 are: r : 0 1 2 3 4 5 6 7 a : 0 2 2 4 6 6 8 8 The
 *            sequence is symmetric: a(n) = a(n-14) and a(14‑r)=a(r). Hence
 *            for any r>7 we use r = 14‑r to bring it into the 0..7 range. The
 *            implementation uses only integer arithmetic an...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Reduce to the period [0,13] */
    idx_t r = n % 14;          /* n is unsigned, 14 fits in 64‑bit */
    if (r > 7) r = 14 - r;     /* use symmetry: 0..7 only */

    /* Map r to a(n) */
    seq_t val;
    switch (r) {
        case 0:  val = 0; break;
        case 1:
        case 2:  val = 2; break;
        case 3:  val = 4; break;
        case 4:
        case 5:  val = 6; break;
        default: /* r == 6 or r == 7 */ val = 8; break;
    }
    return val;
}
