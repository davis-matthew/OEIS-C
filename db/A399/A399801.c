/*
 * A399801  a(1) = 1; a(n+1) is the smallest integer greater than a(n) such
 *          that: a(k) + a(k+2) is not a term for any k.
 * https://oeis.org/A399801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3*n-1)/2) - (n mod 2)*(m + (1-m)*(floor((n+1)/2)
 *            mod 2)) + [n=1], where m = floor(log₂(n)) mod 2
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Compute OEIS sequence A399801.
   a(1) = 1; for n >= 2:
   - If n is even: a(n) = floor((3*n-1)/2).
   - If n is odd:   a(n) = floor((3*n-1)/2) - (m + (1-m)*(floor((n+1)/2) mod 2)),
     with m = floor(log₂(n)) mod 2.
   The special case n==1 returns 1.
*/

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;                /* a(1) = 1; n==0 is irrelevant */

    /* base = floor((3*n-1)/2) */
    seq_t base = (((seq_t)3 * (seq_t)n) - (seq_t)1) / (seq_t)2;

    /* even indices are trivial */
    if ((n & (idx_t)1) == 0) return base;

    /* compute m = floor(log₂(n)) mod 2 */
    idx_t tmp = n;
    idx_t lg = 0;
    while (tmp >>= 1) ++lg;                      /* lg = floor(log₂(n)) */
    idx_t m = lg & (idx_t)1;                     /* parity */

    /* half = floor((n+1)/2) */
    idx_t half = (n + (idx_t)1) >> 1;
    idx_t half_mod2 = half & (idx_t)1;           /* (floor((n+1)/2) mod 2) */

    /* extra = m + (1-m) * half_mod2 */
    idx_t extra = m + (( (idx_t)1 - m ) * half_mod2);

    return base - (seq_t)extra;
}
