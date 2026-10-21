/*
 * A282078  Number of 10-element subsets of [n+10] having an even sum.
 * https://oeis.org/A282078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{m even, 0≤m≤10} binom(O,m) * binom(E,10−m) where O =
 *            ceil((n+10)/2) is the number of odd elements in [1..n+10], and E
 *            = ⌊(n+10)/2⌋ is the number of even elements
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 39, u64 n <= 404, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t num = n - k + i;
        idx_t den = i;
#ifdef __SIZEOF_INT128__
        __int128 t = (__int128)res * num;
        t /= den;
        res = (seq_t)t;
#else
        res = res * (seq_t)num / (seq_t)den;
#endif
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t N = n + 10;
    idx_t O = (N + 1) / 2;     /* odds in [1..N] */
    idx_t E = N - O;           /* evens in [1..N] */
    seq_t total = 0;
    for (idx_t m = 0; m <= 10; m += 2) {
        if (m > O || (10 - m) > E)
            continue;
        total += binom_idx(O, m) * binom_idx(E, 10 - m);
    }
    return total;
}
