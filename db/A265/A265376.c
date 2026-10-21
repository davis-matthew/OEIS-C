/*
 * A265376  a(1) = 1 and a(n) = Sum_{i=1..n-1} (-1)^i*i*a(i).
 * https://oeis.org/A265376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{i=1}^{n-1} (-1)^i * i * a(i) with a(1) = 1
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A265376.  The function is fully
   self‑contained and works for any seq_t capable of holding
   the required magnitudes.  It runs in O(n) time with minimal
   memory usage. */
seq_t a(idx_t n)
{
    /* Handle the first term explicitly.  For n < 1 the result
       is unspecified, but the offset guarantees n >= 1. */
    if (n == 1)
        return (seq_t)1;

    seq_t a_val = (seq_t)1;     /* a(1) */
    seq_t running_sum = -a_val; /* sum of b(i) up to i=1   // b(1)=(-1)^1*1*a(1) */

    /* Iterate from 2 up to n, computing each term and the
       auxiliary sum of b(i) = (-1)^i * i * a(i).             */
    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t a_k   = running_sum;              /* a(k) = Σ_{i=1}^{k-1} b(i) */
        seq_t sign  = ((k & 1) ? (seq_t)-1 : (seq_t)1);   /* (-1)^k   */
        seq_t b_k   = sign * (seq_t)k * a_k;  /* b(k) = (-1)^k * k * a(k) */
        running_sum += b_k;                   /* update running sum for next step */
        a_val = a_k;                           /* remember last a(k) */
    }

    return a_val;
}
