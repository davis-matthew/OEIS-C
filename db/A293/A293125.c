/*
 * A293125  Expansion of e.g.f.: exp(-x/(1+x)).
 * https://oeis.org/A293125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(2*n-1)*a(n-1) - (n-2)*(n-1)*a(n-2); a(0) = 1; a(1) = -1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 18, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A293125.
   Uses the recurrence:
       a(0) = 1
       a(1) = -1
       a(n) = -(2*n-1)*a(n-1) - (n-2)*(n-1)*a(n-2)   for n ≥ 2
   The type 'seq_t' may be any signed integer type or an arbitrary-precision
   integer class; the type 'idx_t' is an unsigned integer type (typically
   uint64_t).  All arithmetic is done using the operators provided for
   seq_t, with no floating point or external dependencies. */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)(-1);

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)(-1);
    seq_t an = (seq_t)0;

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        seq_t mul1 = ((seq_t)2 * i) - (seq_t)1;                /* 2*i-1   */
        seq_t term1 = - (mul1 * a1);                          /* -(2*i-1)*a(i-1) */
        seq_t mul2 = ((seq_t)(i - (idx_t)2)) * ((seq_t)(i - (idx_t)1)); /* (i-2)*(i-1) */
        seq_t term2 = - (mul2 * a0);                          /* -(i-2)*(i-1)*a(i-2) */
        an = term1 + term2;

        a0 = a1;   /* shift for next iteration */
        a1 = an;
    }

    return an;
}
