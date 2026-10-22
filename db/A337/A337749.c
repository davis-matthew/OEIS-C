/*
 * A337749  a(n) = n! * Sum_{k=0..floor(n/2)} (-1)^k / (n-2*k)!.
 * https://oeis.org/A337749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = a(1) = 1; a(n) = 1 - n*(n-1)*a(n-2) (n ≥ 2)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial base cases */
    if (n == 0u || n == 1u)
        return (seq_t)1;

    /* a0 = a(i-2), a1 = a(i-1) */
    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)1;  /* a(1) */
    seq_t ai;             /* temporary for a(i) */

    for (idx_t i = 2u; i <= n; ++i)
    {
        /* a(i) = 1 - i*(i-1)*a(i-2) */
        ai = (seq_t)1
            - (seq_t)i * (seq_t)(i - 1u) * a0;

        /* shift indices for next iteration */
        a0 = a1;
        a1 = ai;
    }

    return a1; /* a(n) */
}
