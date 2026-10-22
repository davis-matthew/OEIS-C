/*
 * A393216  Number of transitive relations on an n-set with exactly six
 *          ordered pairs.
 * https://oeis.org/A393216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*C(n,3) + 758*C(n,4) + 11600*C(n,5) + 87943*C(n,6) +
 *            393960*C(n,7) + 1137752*C(n,8) + 2172240*C(n,9) +
 *            2696400*C(n,10) + 1995840*C(n,11) + 665280*C(n,12)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n + 1 - i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t res = 0;
    res += 12ULL * binom_calc(n, 3);
    res += 758ULL * binom_calc(n, 4);
    res += 11600ULL * binom_calc(n, 5);
    res += 87943ULL * binom_calc(n, 6);
    res += 393960ULL * binom_calc(n, 7);
    res += 1137752ULL * binom_calc(n, 8);
    res += 2172240ULL * binom_calc(n, 9);
    res += 2696400ULL * binom_calc(n, 10);
    res += 1995840ULL * binom_calc(n, 11);
    res += 665280ULL * binom_calc(n, 12);

    return res;
}
