/*
 * A313124  Coordination sequence Gal.6.250.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-11) - a(n-12) Initial values: a(0)=1,
 *            a(1)=4, a(2)=9, a(3)=14, a(4)=19, a(5)=25, a(6)=29, a(7)=35,
 *            a(8)=40, a(9)=45, a(10)=50, a(11)=54, a(12)=58
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Static cache big enough for all n up to ~10000 */
static seq_t cache[20001] = {
    1, 4, 9, 14, 19, 25, 29, 35, 40, 45, 50, 54, 58
};
static idx_t computed_max = 12;

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n <= computed_max)
        return cache[n];

    for (idx_t i = computed_max + 1; i <= n; ++i)
    {
        cache[i] = cache[i - 1] + cache[i - 11] - cache[i - 12];
    }
    computed_max = n;
    return cache[n];
}
