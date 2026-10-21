/*
 * A272071  Expansion of x*(3 - 2*x + x^2)/((1 - x)^2*(1 + x + x^2)).
 * https://oeis.org/A272071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 2*floor(n/3) - 5*floor((n+1)/3)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-3) - a(n-4) for n > 3
   with a(0)=0, a(1)=3, a(2)=1, a(3)=2 */

seq_t a(idx_t n)
{
    /* Compute the closed‑form expression in 64‑bit arithmetic,
       then cast to seq_t.  The intermediate fits in idx_t
       for the range of n of interest (up to at least 10 000). */
    idx_t val = 3 * n
                - 2 * (n / 3)
                - 5 * ((n + 1) / 3);
    return (seq_t)val;
}
