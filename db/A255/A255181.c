/*
 * A255181  Third differences of seventh powers (A001015).
 * https://oeis.org/A255181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0 * a(n) = 125 if n = 1 * a(n) = 42 * (5*n^4 -
 *            10*n^3 + 15*n^2 - 10*n + 3) for n > 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 67, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A255181. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)1;
    }
    if (n == (idx_t)1) {
        return (seq_t)125;
    }

    /* convert n to seq_t to avoid truncation in multiplications */
    seq_t t   = (seq_t)n;          /* n          */
    seq_t t2  = t * t;             /* n^2        */
    seq_t t3  = t2 * t;            /* n^3        */
    seq_t t4  = t2 * t2;           /* n^4        */

    /* A = 5*n^4 + 15*n^2 + 3 */
    seq_t A = (seq_t)5 * t4 + (seq_t)15 * t2 + (seq_t)3;

    /* B = 10*n^3 + 10*n */
    seq_t B = (seq_t)10 * t3 + (seq_t)10 * t;

    /* diff = A - B  (always positive for n > 1) */
    seq_t diff = A - B;

    /* result = 42 * diff */
    return diff * (seq_t)42;
}
