/*
 * A271713  Numbers k such that 3*k - 5 is a square.
 * https://oeis.org/A271713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n + floor((n-1)/2))^2 + 5) / 3 where a(1)=2
 * Range:     u8 n <= 10, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the nth term of OEIS A271713 (numbers k such that 3*k-5 is a square) */
seq_t a(idx_t n)
{
    if (n == 0)   /* Undefined for n = 0, but return 0 to be safe */
        return 0;

    /* Compute the nth integer t that is not a multiple of 3:
       t_n = n + floor((n-1)/2)   (pattern 1,2,4,5,7,8,...)      */
    idx_t half  = (n - 1) / 2;          /* integer division, n>=1 so no underflow */
    seq_t t     = (seq_t)n + (seq_t)half;

    /* Compute k = (t^2 + 5) / 3.  The division is exact because t^2 ≡ 1 (mod 3). */
    seq_t t2    = t * t;
    seq_t k     = (t2 + (seq_t)5) / (seq_t)3;
    return k;
}
