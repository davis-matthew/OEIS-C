/*
 * A247303  Convolution of A010059(n) with itself.
 * https://oeis.org/A247303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^n (1-A010060(k))·(1-A010060(n-k)) Where
 *            A010060(k) = 1 if k has an odd number of 1-bits (odious), so
 *            (1-A010060(k)) = 1 if k is evil (even number of 1-bits)
 * Range:     u8 n <= 638, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by counting ordered pairs of evil numbers that sum to n. */
seq_t a(idx_t n)
{
    seq_t res = 0;                     /* accumulator */
    for (idx_t k = 0; k <= n; ++k)     /* iterate over all splits k + (n-k) */
    {
        /* Identify evil numbers: even popcount of binary representation. */
        if ( (__builtin_popcountll((unsigned long long)k) & 1ULL) == 0ULL &&
             (__builtin_popcountll((unsigned long long)(n - k)) & 1ULL) == 0ULL )
        {
            res += 1;                  /* one ordered pair found */
        }
    }
    return res;
}
