/*
 * A361079  Number of integers in [n .. 2n-1] having the same binary weight as
 *          n.
 * https://oeis.org/A361079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = |{ k in [n .. 2n-1] : popcount(k) = popcount(n) }|
 * Range:     u8 n <= 1054, u16 n <= 9776, u32 n <= 9776, u64 n <= 9776,
 *            u128 n <= 9776, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline unsigned int popcount_u64(idx_t x)
{
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    unsigned int target_weight = popcount_u64(n);
    idx_t limit = n + n;            /* 2*n, same as (n<<1) */
    for (idx_t k = n; k < limit; ++k) {
        if (popcount_u64(k) == target_weight)
            ++count;
    }
    return count;
}
