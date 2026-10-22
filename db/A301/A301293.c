/*
 * A301293  Expansion of (x^2+x+1)^2 / ((x^2+1)*(x-1)^2).
 * https://oeis.org/A301293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Formula: a(n) = 1 for n=0; for n>0  
   a(n) = 9*n/2          if n even,  
   a(n) = 9*(n-1)/2 + 4 + ((n-1)/2 mod 2)  if n odd
   (equivalently, a(n) = 9k + 4 + (k%2) with k = (n-1)/2 for odd n) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if ((n & (idx_t)1) == (idx_t)0)          /* even n */
    {
        seq_t half = ((seq_t)n) / (seq_t)2;
        return ((seq_t)9) * half;
    }
    else                                      /* odd n */
    {
        idx_t k = (n - 1) / 2;
        return ((seq_t)9) * (seq_t)k + (seq_t)4 + (seq_t)(k & (idx_t)1);
    }
}
