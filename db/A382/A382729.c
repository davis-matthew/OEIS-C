/*
 * A382729  Total number of entries in rows 0,1,...,n of Pascal's triangle not
 *          divisible by 4.
 * https://oeis.org/A382729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{r=0}^n |{k : 0 ≤ k ≤ r and v₂(C(r,k)) < 2}|
 * Range:     u8 n <= 26, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses Kummer's theorem: v₂(C(r,k)) = popcnt(k) + popcnt(r-k) - popcnt(r) */

static inline unsigned long long popcnt_u64(unsigned long long x)
{
    return __builtin_popcountll(x);
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t r = 0; r <= n; ++r)
    {
        unsigned long long rpop = popcnt_u64(r);
        for (idx_t k = 0; k <= r; ++k)
        {
            unsigned long long exp =
                popcnt_u64(k) + popcnt_u64(r - k) - rpop;
            if (exp < 2)
                ++sum;
        }
    }
    return sum;
}
