/*
 * A366511  a(n) = Lucas(2*n) + 2^(n + 1) + 1.
 * https://oeis.org/A366511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(2*n) + 2^(n+1) + 1 Lucas(2*n) = 2*F_{2n+1} –
 *            F_{2n}, where F_k is the Fibonacci number (F_0 = 0, F_1 = 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute F_n and F_{n+1} by fast doubling.  The recursion depth is
   O(log n), suitable for all n up to at least 10 000.  The function
   works for any unsigned integer type seq_t that supports +, -, *, <<,
   >> and assignment. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b);          /* a = F_k, b = F_{k+1}  */

    /* c = F_{2k}   = F_k * (2*F_{k+1} – F_k)   */
    seq_t two_b = b + b;              /* equals 2*F_{k+1} */
    c = a * (two_b - a);

    /* d = F_{2k+1} = F_{k+1}^2 + F_k^2             */
    d = a * a + b * b;

    if ((n & 1ULL) == 0) {            /* even n  */
        *fn  = c;
        *fn1 = d;
    } else {                          /* odd n   */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)                     /* sequence is defined only for n ≥ 1 */
        return 0;

    /* Compute F_{2n} and F_{2n+1}.  Note that 2*n fits in idx_t. */
    seq_t f2n, f2n1;
    fib_pair(2 * n, &f2n, &f2n1);

    /* Lucas(2n) = 2*F_{2n+1} – F_{2n}.  The subtraction is safe because
       for Fibonacci numbers we have 2*F_{2n+1} ≥ F_{2n}. */
    seq_t lucas = (f2n1 + f2n1) - f2n;   /* equals (f2n1 << 1) – f2n */

    /* Compute 2^(n+1) by repeated doubling (safe for all unsigned types). */
    seq_t pow_term = (seq_t)1;
    for (idx_t i = 0; i < n + 1; ++i)
        pow_term += pow_term;           /* multiply by 2 each iteration */

    return lucas + pow_term + (seq_t)1;
}
