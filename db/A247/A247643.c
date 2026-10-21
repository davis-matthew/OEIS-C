/*
 * A247643  a(n) = ( 10*n*(n+1)+(2*n+1)*(-1)^n+7 )/8.
 * https://oeis.org/A247643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 146, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (10*n*(n+1)+(2*n+1)*(-1)^n+7)/8
 * Offset: 0  (a(0) is the first term)
 * Computes the OEIS sequence A247643.
 */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t term = 10 * nn * (nn + 1) + 7;  /* base part of numerator */

    if (n & 1) {                  /* n odd: subtract (2*n+1) */
        term -= 2 * nn + 1;
    } else {                      /* n even: add   (2*n+1) */
        term += 2 * nn + 1;
    }

    /* division by 8 yields the integer term */
    return term / 8;
}
