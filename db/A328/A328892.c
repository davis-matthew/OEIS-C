/*
 * A328892  If n = Product (p_j^k_j) then a(n) = Sum (2^(k_j - 1)).
 * https://oeis.org/A328892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum(2^(k_j-1)) over prime powers p_j^k_j dividing n
 *            (a(1)=0). The function factorizes n by trial division and sums
 *            the contributions 2^(exponent-1). The implementation runs in
 *            O(sqrt(n)) time and uses only integer operations
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    uint64_t m = (uint64_t)n;
    seq_t   ans = 0;

    /* factor 2 separately (most frequent case) */
    if ((m & 1ULL) == 0) {
        int exp = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            exp++;
        }
        seq_t term = 1;
        for (int i = 0; i < exp - 1; ++i)
            term <<= 1;
        ans += term;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            seq_t term = 1;
            for (int i = 0; i < exp - 1; ++i)
                term <<= 1;
            ans += term;
        }
    }

    /* remaining prime factor, if any (exponent = 1) */
    if (m > 1)
        ans += 1;          /* 2^(1-1) = 1 */

    return ans;
}
