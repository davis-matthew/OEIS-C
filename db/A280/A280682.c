/*
 * A280682  Integers m such that floor(sqrt(m)) is even.
 * https://oeis.org/A280682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2t)^2 + (n - t*(2t-1) - 1), where t = smallest
 *            non‑negative integer with (t+1)(2t+1) >= n
 * Range:     u8 n <= 120, u16 n <= 4950, u32 n <= 4950, u64 n <= 4950,
 *            u128 n <= 4950, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A280682: integers m such that floor(sqrt(m)) is even */

static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    /* Highest power of two <= 2^63, since (1<<63) would overflow unsigned 64 */
    for (idx_t bit = (idx_t)1 << 62; bit; bit >>= 1) {
        idx_t cand = r | bit;
        if ((unsigned __int128)cand * cand <= x)
            r = cand;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)       /* the sequence is defined only for n >= 1, but keep safe */
        return (seq_t)0;

    /* Find the block index t such that (t+1)(2t+1) >= n. */
    idx_t t = 0;
    /* Rough initial guess from the quadratic formula: t ≈ (-3 + sqrt(1+8n))/4. */
    idx_t sq = isqrt_u64(1 + 8 * n);
    if (sq > 3)
        t = (sq - 3) / 4;

    /* Adjust in case the guess was low. */
    while ((unsigned __int128)(t + 1) * (2 * t + 1) < n)
        ++t;

    /* offset within the block */
    idx_t prev_sum = t * (2 * t - 1);   /* S(t-1) = t*(2t-1), works for t==0 */
    idx_t offset   = n - prev_sum - 1;  /* 0‑based index inside the block */

    /* Compute the starting value of the block: (2t)^2 */
    seq_t k     = (seq_t)(2 * (seq_t)t);   /* even floor(sqrt) value        */
    seq_t base  = k * k;                    /* k^2 (start of block)          */

    return base + (seq_t)offset;
}
