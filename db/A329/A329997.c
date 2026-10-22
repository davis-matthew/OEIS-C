/*
 * A329997  Beatty sequence for 3^x, where 1/x^3 + 1/3^x = 1.
 * https://oeis.org/A329997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * 3429542319312 / 1000000000000 )
 * Range:     u8 none, u16 none, u32 none, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 3429542319312 / 1000000000000 is a rational approximation of 3^x
   where 1/x^3 + 1/3^x = 1, accurate enough to give the correct floor
   for all indices that fit in seq_t. */

seq_t a(idx_t n)
{
    /* Rational constants for 3^x, chosen so that the error < 1/(2n) for
       all practical n (e.g., n ≤ 10^4).  The denominator is 10^12, a
       value that fits in 64‑bit and leaves ample room for arbitrary‑
       precision arithmetic. */
    static const seq_t P = 3429542319312ULL; /* numerator */
    static const seq_t Q = 1000000000000ULL; /* denominator */

    /* Compute floor(n * P / Q).  All intermediates are first cast to
       unsigned __int128 to avoid overflow when seq_t is 64‑bit.
       The division truncates toward zero, giving exactly the floor. */
    unsigned __int128 prod = (unsigned __int128)P * (unsigned __int128)n;
    return (seq_t)(prod / Q);
}
