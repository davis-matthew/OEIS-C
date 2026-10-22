/*
 * A307182  Crossing number of the n-crown graph (conjectured).
 * https://oeis.org/A307182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-2)^2*(n-4)/16 for n even; a(n) = (n-1)^2*(n-3)^2/16
 *            for n odd (OEIS A307182)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Terms 1..4 are zero; they are handled explicitly to avoid
       unsigned wrap‑around on the subtractions. */
    if (n < 5)
        return 0;

    if ((n & 1) == 0) {              /* n is even */
        seq_t nn = (seq_t)n;
        /* n*(n-2)^2*(n-4)/16  – guaranteed divisible by 16 for even n */
        seq_t val = nn * (nn - 2) * (nn - 2) * (nn - 4);
        return val / 16;
    } else {                          /* n is odd */
        seq_t nn = (seq_t)n;
        /* (n-1)^2*(n-3)^2/16  – guaranteed divisible by 16 for odd n */
        seq_t val = (nn - 1) * (nn - 1) * (nn - 3) * (nn - 3);
        return val / 16;
    }
}
