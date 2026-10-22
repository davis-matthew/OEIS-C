/*
 * A359844  a(n) = ((2*n+1)^8 + 1)/2.
 * https://oeis.org/A359844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*n+1)^8 + 1)/2
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 8, u64 n <= 139, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t = t * 2 + 1;                /* t = 2*n+1 */

#if defined(__SIZEOF_INT128__)
    /* If seq_t is <= 64 bits, use 128‑bit intermediates to avoid overflow. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 v = (unsigned __int128)t;
        unsigned __int128 r = v;
        for (int i = 1; i < 8; ++i)
            r *= v;
        return (seq_t)((r + 1) / 2);
    } else
#endif
    {
        seq_t r = t;
        for (int i = 1; i < 8; ++i)
            r *= t;
        return (r + 1) / 2;
    }
}
