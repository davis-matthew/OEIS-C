/*
 * A285869  a(n) is the number of zeros of the Chebyshev S(n, x) polynomial in
 *          the open interval (-sqrt(2), +sqrt(2)).
 * https://oeis.org/A285869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*b(n) if n is even, else a(n) = 1 + 2*b(n); where b(n) =
 *            floor(n/2) - floor((n+1)/4)
 * Range:     u8 n <= 509, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(n/2) and floor((n+1)/4) without overflow. */
    seq_t half = (seq_t)(n >> 1);
    seq_t div4 = (seq_t)(n >> 2);
    seq_t r     = (seq_t)(n & 3);
    seq_t floor_np1_div4 = div4 + ((r + 1) >> 2);

    seq_t b = half - floor_np1_div4;
    if ((n & 1) == 0)
        return (seq_t)2 * b;        /* even n */
    else
        return (seq_t)1 + (seq_t)2 * b; /* odd n */
}
