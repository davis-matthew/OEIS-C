/*
 * A307849  Number of ways to pay n dollars using Canadian coins, that is:
 *          nickels (5 cents), dimes (10 cents), quarters (25 cents), loonies
 *          (100 cents or $1 coins) and toonies ($2 coins).
 * https://oeis.org/A307849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (33 - 9*(-1)^n + 224*n + 298*n^2 + 136*n^3 + 20*n^4) / 24
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 144, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t for arithmetic */
    seq_t sn = (seq_t)n;

    /* compute powers of n */
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    seq_t n4 = n3 * sn;

    /* base polynomial part of the numerator */
    seq_t base = (seq_t)33
                 + (seq_t)224 * sn
                 + (seq_t)298 * n2
                 + (seq_t)136 * n3
                 + (seq_t)20  * n4;

    /* add the parity adjustment: +9 if n is odd, -9 if n is even */
    if ((n & 1) == 0) {
        base -= (seq_t)9;
    } else {
        base += (seq_t)9;
    }

    /* exact division by 24 (numerator is always a multiple of 24) */
    return base / (seq_t)24;
}
