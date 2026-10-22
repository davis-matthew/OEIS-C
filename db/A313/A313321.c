/*
 * A313321  Coordination sequence Gal.3.42.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For n > 9, a(n) = 2*a(n-4) - a(n-8) Initial terms: a(0)=1,
 *            a(1)=4, a(2)=9, a(3)=16, a(4)=18, a(5)=24, a(6)=31, a(7)=34,
 *            a(8)=38, a(9)=42
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t cache[20001];
    static idx_t computed = 0;   /* number of computed entries (>=10 after init) */
    if (computed == 0) {
        const seq_t base[10] = {1,4,9,16,18,24,31,34,38,42};
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = base[i];
        computed = 10;
    }
    if (n < computed)
        return cache[n];
    for (idx_t i = computed; i <= n; ++i) {
        /* recurrence holds for i >= 10; i-4 and i-8 are valid indices */
        seq_t t = cache[i-4] << 1;   /* 2 * cache[i-4] */
        cache[i] = t - cache[i-8];
    }
    computed = n + 1;
    return cache[n];
}
