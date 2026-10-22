/*
 * A361751  a(n) is the number of decimal digits in A098129(n) and A300517(n).
 * https://oeis.org/A361751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{k=0..⌊log10(n)⌋} (n*(n+1) - 10^k*(10^k-1)) / 2
 * Range:     u8 n <= 16, u16 n <= 216, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of decimal digits in A098129(n) and A300517(n). */
seq_t a(idx_t n)
{
    /* n is at least 1 in the OEIS offset */
    seq_t sum = 0;
    idx_t pow10 = 1;           /* 10^k, grows each iteration                       */
    for (int k = 0; pow10 <= n; ++k) {
        /* compute (n*(n+1) - pow10*(pow10-1)) / 2  as seq_t  */
        seq_t term = ((seq_t)n * ((seq_t)n + 1)
                     - (seq_t)pow10 * ((seq_t)pow10 - 1)) / 2;
        sum += term;

        /* prepare next pow10: stop when further multiplication would overflow or exceed n */
        if (pow10 > n / 10)   /* pow10*10 would be > n or overflow  */
            break;
        pow10 *= 10;
    }
    return sum;
}
