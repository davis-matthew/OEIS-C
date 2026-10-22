/*
 * A395933  Number of fixed points of the longest-run operator with radius 2
 *          on binary configurations of the cycle graph C_n.
 * https://oeis.org/A395933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - a(n-6) Initial values:
 *            a(1)=2, a(2)=4, a(3)=2, a(4)=8, a(5)=12, a(6)=22
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache[0] is unused; indices start at 1 */
static seq_t cache[10001];            /* sufficient for n up to about 10000 */
static idx_t cache_len = 0;           /* highest index for which cache[i] holds a correct value */

/* initialise the first six known terms of the sequence */
static void
ensure_base(void)
{
    if (cache_len >= 6) return;
    cache[1] = 2;
    cache[2] = 4;
    cache[3] = 2;
    cache[4] = 8;
    cache[5] = 12;
    cache[6] = 22;
    cache_len = 6;
}

seq_t
a(idx_t n)
{
    if (n < 1) return 0;          /* Undefined for n < 1, return 0 */
    ensure_base();                /* guarantees indices 1..6 exist */

    if (n <= cache_len) return cache[n];

    /* Compute values up to n if not already computed */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t term = 2 * cache[i - 1];   /* 2*a(i-1) */
        term -= 2 * cache[i - 3];        /* - 2*a(i-3) */
        term += 2 * cache[i - 4];        /* + 2*a(i-4) */
        term -= cache[i - 6];            /* - a(i-6) */
        cache[i] = term;
    }
    cache_len = n;
    return cache[n];
}
