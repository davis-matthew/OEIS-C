/*
 * A308123  Sum of the perimeters of all integer-sided isosceles triangles
 *          with perimeter n.
 * https://oeis.org/A308123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * ( floor((n-1)/2) - floor(n/4) )
 * Range:     u8 n <= 32, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the number of integer‑sided isosceles triangles with perimeter n
   times n (the perimeter).  It is the same as A059169(n) * n. */
seq_t a(idx_t n)
{
    /* both r1 and r2 fit in idx_t (uint64_t) */
    idx_t r1 = (n == 0) ? 0 : (n - 1) / 2;  /* floor((n-1)/2) */
    idx_t r2 = n / 4;                      /* floor(n/4)     */

    seq_t count;
    if (r1 < r2)
        count = 0;
    else
        count = (seq_t)(r1 - r2);          /* number of isosceles triangles */

    return (seq_t)n * count;               /* sum of perimeters */
}
