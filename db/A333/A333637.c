/*
 * A333637  The number of cells which contain multiple squares of a
 *          Genealodron formed from 2^n - 1 equal-sized squares (when viewed
 *          from above).
 * https://oeis.org/A333637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 (n=1,2,3), 2 (n=4), 7 (n=5), n^2 - n - 15 (n>=6)
 * Range:     u8 n <= 16, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small n explicitly (offset 1) */
    if (n <= 5) {
        /* Use a switch to avoid array access when seq_t is very small */
        switch (n) {
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
            case 3: return (seq_t)0;
            case 4: return (seq_t)2;
            case 5: return (seq_t)7;
            default: return (seq_t)0; /* unreachable */
        }
    }
    /* For n >= 6 use the clean quadratic formula */
    seq_t nn = (seq_t)n;          /* cast first to avoid overflow of the product   */
    return nn * nn - nn - (seq_t)15;
}
