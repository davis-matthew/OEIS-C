/*
 * A312289  Coordination sequence Gal.6.527.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-7) - a(n-8) for n > 8, initial values:
 *            a(0)=1, a(1)=4, a(2)=8, a(3)=14, a(4)=16, a(5)=22, a(6)=26,
 *            a(7)=30, a(8)=34
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define CACHE_MAX 200000  /* enough for all n requested in the tests */

static seq_t cache[CACHE_MAX];
static idx_t computed = (idx_t)-1;  /* -1 indicates uninitialised */

seq_t a(idx_t n)
{
    /* initialise cache only once */
    if (computed == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)16;
        cache[5] = (seq_t)22;
        cache[6] = (seq_t)26;
        cache[7] = (seq_t)30;
        cache[8] = (seq_t)34;
        computed = 8;
    }

    if (n <= computed)
        return cache[n];

    /* grows cache if necessary (very unlikely, n ≤ 10000 is expected) */
    if (n >= CACHE_MAX) {
        /* for safety, clamp to the maximum supported index */
        n = CACHE_MAX - 1;
    }

    for (idx_t i = computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1) + a(i-7) - a(i-8) */
        cache[i] = cache[i - 1] + cache[i - 7] - cache[i - 8];
    }
    computed = n;
    return cache[n];
}
