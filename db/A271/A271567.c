/*
 * A271567  Convolution of nonzero triangular numbers (A000217) and nonzero
 *          tetradecagonal numbers (A051866).
 * https://oeis.org/A271567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(n + 2)*(n + 3)*(n + 4)*(12*n + 5)/120
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 131, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit intermediate to avoid overflow on 64‑bit builds */
    unsigned __int128 tn = (unsigned __int128)n;
    unsigned __int128 t  = (tn + 1) * (tn + 2) * (tn + 3) * (tn + 4)
                           * ((unsigned __int128)12 * tn + 5);
    t /= 120;
    return (seq_t)t;
}
