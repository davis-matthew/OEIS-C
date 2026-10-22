/*
 * A358607  a(n) = Sum_{k=0..floor(n/2)} (-1)^k * (n-2*k)!.
 * https://oeis.org/A358607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} (-1)^k · (n-2k)!; recurrence for n>2:
 *            a(0)=1, a(1)=1, a(2)=1, a(n) = n·a(n-1) – a(n-2) + n·a(n-3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* static cache for n up to a modest size (enough for the tests) */
    enum { CACHE_SIZE = 20001 };
    static seq_t cache[CACHE_SIZE];
    static idx_t last_cached = (idx_t)-1;
    static int initialized = 0;

    if (!initialized) {
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        last_cached = 2;
        initialized = 1;
    }

    if (n < CACHE_SIZE) {
        /* Return cached value if already computed */
        if (n <= last_cached) return cache[n];

        /* Compute all missing terms up to n using the recurrence */
        for (idx_t i = last_cached + 1; i <= n; ++i) {
            seq_t t1 = (seq_t)i * cache[i-1];
            seq_t t2 = cache[i-2];
            seq_t t3 = (seq_t)i * cache[i-3];
            cache[i] = t1 - t2 + t3;
        }
        last_cached = n;
        return cache[n];
    }

    /* For n beyond the cache, compute on the fly without caching
       (this situation is outside the typical test range). */
    if (n == 0 || n == 1 || n == 2) return 1;

    seq_t a0 = 1, a1 = 1, a2 = 1, a_next;
    for (idx_t i = 3; i <= n; ++i) {
        a_next = (seq_t)i * a2 - a1 + (seq_t)i * a0;
        a0 = a1;
        a1 = a2;
        a2 = a_next;
    }
    return a2;
}
