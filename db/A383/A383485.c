/*
 * A383485  a(n) = 2*(2*(n - 1)! + n + 2) mod (n*(n + 2)).
 * https://oeis.org/A383485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(2*(n - 1)! + n + 2) mod (n*(n + 2))
 * Range:     u8 n <= 12, u16 n <= 4308, u32 n <= 3621, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial case n == 1 directly */
    if (n == 1) {
        return 1;              /* 10 mod 3 == 1 */
    }

    seq_t nseq = (seq_t)n;                      /* convert index to seq_t */
    seq_t denom = nseq * (nseq + 2);            /* modulus: n*(n+2) */

    /* Compute (n-1)! modulo denom */
    seq_t fact = 1;
    for (idx_t i = 2; i < n; ++i) {
        fact = (fact * (seq_t)i) % denom;
    }

    /* Compute 2*(2*fact + n + 2) modulo denom */
    seq_t term = 2 * fact;                   /* 2*(n-1)!  */
    term += nseq + 2;                       /* + n + 2   */
    term *= 2;                              /* times 2    */
    term %= denom;                          /* final modulo */

    return term;
}
