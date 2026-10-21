/*
 * A284362  a(n) = Sum_{d|n, d = 0, 1, or 5 mod 6} d.
 * https://oeis.org/A284362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d ≡ 0,1,5 (mod 6)} d
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute integer square root of a uint64_t using binary search */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* External linkage function: returns sum of divisors of n that are
   congruent to 0, 1, or 5 modulo 6. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n<1, but safe */
    seq_t sum = 0;
    idx_t root = isqrt_u64(n);

    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            /* add d1 if it satisfies the modulo condition */
            idx_t mod1 = d1 % 6;
            if (mod1 == 0 || mod1 == 1 || mod1 == 5)
                sum += (seq_t)d1;

            /* add d2 if it is distinct from d1 and satisfies the condition */
            if (d2 != d1) {
                idx_t mod2 = d2 % 6;
                if (mod2 == 0 || mod2 == 1 || mod2 == 5)
                    sum += (seq_t)d2;
            }
        }
    }

    return sum;
}
