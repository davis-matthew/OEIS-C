/*
 * A245941  a(n) = (16n^6 - 24n^5 + 2n^4 + 11n^3 - 6n^2 + n) / 6.
 * https://oeis.org/A245941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*n*(2*n-1)*(8*n^3-3*n+1)/6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 34, u64 n <= 1380,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small cases directly to avoid under‑flow of unsigned arithmetic */
    if (n < 2)
        return 0;

    seq_t N  = (seq_t)n;          /* cast idx_t to seq_t explicitly          */

    /* Individual factors of the product */
    seq_t f1 = N;                            /* n                         */
    seq_t f2 = N - (seq_t)1;                 /* n-1                       */
    seq_t f3 = (seq_t)2 * N - (seq_t)1;       /* 2*n-1                     */

    /* n^2 and n^3 – still safe for the n values used in the tests */
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;

    /* 8*n^3 - 3*n + 1  (f4) */
    seq_t f4 = (seq_t)8 * n3 - (seq_t)3 * N + (seq_t)1;

    /* Divide the product by 6 by handing the 2 and the 3 factors to the
       individual factors.  At least one of f1,f2,f3 is even, and at least
       one of f1,f2,f3 is divisible by 3, so these divisions are always
       exact. */
    if ((f1 & (seq_t)1) == 0)
        f1 /= (seq_t)2;
    else if ((f2 & (seq_t)1) == 0)
        f2 /= (seq_t)2;
    else if ((f3 & (seq_t)1) == 0)
        f3 /= (seq_t)2;

    if (f1 % (seq_t)3 == 0)
        f1 /= (seq_t)3;
    else if (f2 % (seq_t)3 == 0)
        f2 /= (seq_t)3;
    else if (f3 % (seq_t)3 == 0)
        f3 /= (seq_t)3;

    /* Final multiplication – after the divisions the product equals the
       sequence value and fits in seq_t for all indices whose result
       fits into seq_t. */
    seq_t result = f1 * f2;
    result *= f3;
    result *= f4;

    return result;
}
