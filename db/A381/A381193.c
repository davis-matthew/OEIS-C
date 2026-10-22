/*
 * A381193  a(n) = (3*n-1)*(n^4-18*n^3+179*n^2-582*n+720)/120.
 * https://oeis.org/A381193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n-1)*(n^4-18*n^3+179*n^2-582*n+720)/120
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 180, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses a 128‑bit intermediate to avoid overflow on 64‑bit builds. */
seq_t a(idx_t n)
{
    /* Promote index to 128‑bit integer for safe intermediate calculations */
    __int128 nn = n;
    __int128 term1 = 3*nn - 1;                                    /* (3*n-1)             */
    __int128 poly  = nn*nn*nn*nn  /* n^4 */
                  - 18*nn*nn*nn   /* -18*n^3 */
                  + 179*nn*nn     /* +179*n^2 */
                  - 582*nn        /* -582*n   */
                  + 720;          /* +720      */
    __int128 prod  = term1 * poly;                                /* full numerator      */
    prod /= 120;                                                 /* divide by denominator*/
    return (seq_t)prod;                                          /* cast back to seq_t    */
}
