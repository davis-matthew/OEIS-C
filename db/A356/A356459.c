/*
 * A356459  a(n) = n! * Sum_{k=1..n} Sum_{d|k} d/(k/d)!.
 * https://oeis.org/A356459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} Sum_{d|k} d/(k/d)!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the identity */
/*   a(n) = Sum_{m=1..n} (n!/m!) * (n/m)(n/m+1)/2 */
/* The term n!/m! is computed incrementally as a suffix product. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset starts at 1; input 0 unused */

    seq_t ratio = (seq_t)1;       /* product_{t=m+1}^{n} t, starts with m = n */
    seq_t sum   = (seq_t)0;

    for (idx_t m = n; m > 0; --m)
    {
        idx_t t = n / m;                          /* floor(n/m) */
        seq_t tt = ((seq_t)t * (seq_t)(t + 1)) / (seq_t)2;   /* t*(t+1)/2 */
        sum += ratio * tt;                        /* contribution for this m */

        if (m > 1)                                 /* prepare ratio for next m-1 */
            ratio *= (seq_t)m;
    }

    return sum;
}
