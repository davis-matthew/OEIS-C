/*
 * A315199  Coordination sequence Gal.6.582.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6n for n % 8 == 0, n>0; a(n) = 5n for even n; a(n) = 5n +
 *            1 - 4*((n-1)/8) for n % 8 == 1; a(n) = 5n - 1 - 4*((n-3)/8) for
 *            n % 8 == 3; a(n) = 5n - 3 - 4*((n-5)/8) for n % 8 == 5; a(n) =
 *            5n - 5 - 4*((n-7)/8) for n % 8 == 7; a(0) = 1
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t n_t = (seq_t)n;
    idx_t r = n & 7;                 /* n % 8 */
    if (r == 0)                      /* n divisible by 8, n > 0 */
        return n_t * (seq_t)6;
    if ((r & 1) == 0)                /* even n (2 or 6 modulo 8) */
        return n_t * (seq_t)5;
    /* odd n: use the appropriate offset */
    idx_t block = (n - r) / 8;        /* number of complete 8‑step blocks before n */
    seq_t base = n_t * (seq_t)5;
    switch (r)
    {
        case 1: return base + (seq_t)1 - (seq_t)4 * (seq_t)block;
        case 3: return base - (seq_t)1 - (seq_t)4 * (seq_t)block;
        case 5: return base - (seq_t)3 - (seq_t)4 * (seq_t)block;
        case 7: return base - (seq_t)5 - (seq_t)4 * (seq_t)block;
    }
    return 0;   /* unreachable */
}
