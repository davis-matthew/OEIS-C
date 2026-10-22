/*
 * A350050  a(n) = (2*n^4 - 6*(-1)^n*n^2 - 2*n^2 + 3*(-1)^n - 3)/96.
 * https://oeis.org/A350050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^4 - 6*(-1)^n*n^2 - 2*n^2 + 3*(-1)^n - 3)/96
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 305, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternative integer formulas
   a(n) = (n^2*(n^2-4))/48            if n is even
   a(n) = (n^2*(n^2+2)-3)/48          if n is odd
   These avoid negative intermediates and work for all n >= 0. */
seq_t a(idx_t n)
{
    /* Convert n to seq_t once for arithmetic. */
    seq_t sn = (seq_t)n;
    /* Compute n^2. */
    seq_t sq = sn * sn;             /* n^2 */
    seq_t res;
    if ((n & 1U) == 0) {            /* n even */
        /* res = n^2*(n^2-4) */
        res = sq * (sq - (seq_t)4);
    } else {                        /* n odd */
        /* res = n^2*(n^2+2) - 3 */
        res = sq * (sq + (seq_t)2) - (seq_t)3;
    }
    /* Divide by 48 to obtain the term. */
    return res / (seq_t)48;
}
