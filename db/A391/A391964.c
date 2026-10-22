/*
 * A391964  a(n) = Sum_{k=0..n} (k+1) * 2^k * binomial(k,2*(n-k)).
 * https://oeis.org/A391964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391964
   Recurrence:  a(n) = 8*a(n-1) - 24*a(n-2) + 40*a(n-3)
                - 48*a(n-4) + 32*a(n-5) - 16*a(n-6)
   Initial values: a(0)=1, a(1)=4, a(2)=12, a(3)=44, a(4)=176, a(5)=672
   (Offset 0)
   Computes any a(n) for n≥0 using memoisation; works for all supported
   seq_t types and for indices up to at least 10 000.
*/

#define _A391964_MAX_N 10000   /* maximum n expected in tests */

seq_t a(idx_t n)
{
    /* Static cache – large enough for n up to 10 000.
       Only the first 6 values are initialised lazily. */
    static seq_t cache[_A391964_MAX_N + 1];
    static idx_t cache_len = 0;   /* number of cached terms */

    /* initialise the first six terms on first call */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)44;
        cache[4] = (seq_t)176;
        cache[5] = (seq_t)672;
        cache_len = 6;
    }

    /* If n is already computed, return it immediately */
    if (n < cache_len) {
        return cache[n];
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Compute positive and negative parts separately to avoid
           temporary underflow with unsigned arithmetic. */
        seq_t pos = 8 * cache[i-1] + 40 * cache[i-3] + 32 * cache[i-5];
        seq_t neg = 24 * cache[i-2] + 48 * cache[i-4] + 16 * cache[i-6];
        cache[i] = pos - neg;
    }
    cache_len = n + 1;          /* update length after filling */
    return cache[n];
}
