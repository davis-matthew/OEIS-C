/*
 * A304588  Length of shortest prefix of the Thue-Morse word (A010060) such
 *          that some length-n block appears twice.
 * https://oeis.org/A304588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ceil(3*nextPow2(n)/2) where nextPow2(n) is the
 *            smallest power of 2 ≥ n
 * Range:     u8 n <= 64, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the next power of two ≥ n (n ≥ 1). */
static idx_t next_pow2(idx_t n)
{
    if (n <= 1) return 1;
    idx_t m = n - 1;
    m |= m >> 1;
    m |= m >> 2;
    m |= m >> 4;
    m |= m >> 8;
    m |= m >> 16;
    /* For 64‑bit indices: */
    m |= m >> 32;
    return m + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1; just in case */
    idx_t t = next_pow2(n);
    seq_t diff;
    if (t == (idx_t)1)
        diff = (seq_t)2;               /* ceil(3*1/2) = 2   */
    else
        diff = ((seq_t)3 * (seq_t)t) / (seq_t)2; /* t is even */
    return (seq_t)n + diff;
}
