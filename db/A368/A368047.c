/*
 * A368047  a(n) = (-1)^n * n*(n + 1)*(2*n + (-1)^n * (4*n + 5) + 1) / 12.
 * https://oeis.org/A368047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1)^n * n*(n + 1)*(2*n + (-1)^n * (4*n + 5) + 1) / 12
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified: if n is even -> a(n) = n*(n+1)^2 / 2 */
/*              if n is odd  -> a(n) = n*(n+1)*(n+2) / 6  */
seq_t a(idx_t n)
{
    /* Even n */
    if ((n & 1ULL) == 0) {
        seq_t n_term = (seq_t)n;          /* n as seq_t */
        seq_t half = n_term / 2;          /* n is even, so division is exact */
        seq_t b = n_term + (seq_t)1;      /* n+1  */
        seq_t res = half * b;             /* n/2 * (n+1)   */
        res = res * b;                    /* * (n+1)       */
        return res;
    }
    /* Odd n */
    else {
        seq_t n_term = (seq_t)n;          /* n as seq_t     */
        seq_t a_term = n_term;            /* n              */
        seq_t b_term = n_term + (seq_t)1; /* n+1            */
        seq_t c_term = n_term + (seq_t)2; /* n+2            */

        /* Divide by 2 */
        if (a_term % (seq_t)2 == 0)
            a_term /= 2;
        else if (b_term % (seq_t)2 == 0)
            b_term /= 2;
        else
            c_term /= 2;

        /* Divide by 3 */
        if (a_term % (seq_t)3 == 0)
            a_term /= 3;
        else if (b_term % (seq_t)3 == 0)
            b_term /= 3;
        else
            c_term /= 3;

        /* Multiply remaining factors */
        seq_t res = a_term * b_term;
        res = res * c_term;
        return res;
    }
}
