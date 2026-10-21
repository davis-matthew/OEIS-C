/*
 * A253352  Number of (3+1)X(n+1) 0..1 arrays with every 2X2 subblock diagonal
 *          maximum minus antidiagonal maximum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 8*a(n-2) - 72*a(n-3) + 96*a(n-4) + 128*a(n-5)
 *            - 128*a(n-6) - 64*a(n-7) (for n > 10) Initial values: a(1)=125,
 *            a(2)=735, a(3)=4800, a(4)=31156, a(5)=200740, a(6)=1294016,
 *            a(7)=8338384, a(8)=53745952, a(9)=346434848, a(10)=2233207264
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values, index 0 ⇔ a(1) */
static size_t cache_len = 0;          /* number of elements currently stored */

/* extend cache up to (and including) n */
static void extend_cache(idx_t n)
{
    if (n <= (idx_t)cache_len)
        return;                       /* already computed */

    size_t new_len = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – terminate */
        abort();
    }
    cache = new_cache;

    /* initial values for 1 ≤ n ≤ 10 */
    static const seq_t init[10] = {
        (seq_t)125ULL,
        (seq_t)735ULL,
        (seq_t)4800ULL,
        (seq_t)31156ULL,
        (seq_t)200740ULL,
        (seq_t)1294016ULL,
        (seq_t)8338384ULL,
        (seq_t)53745952ULL,
        (seq_t)346434848ULL,
        (seq_t)2233207264ULL
    };

    for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
        size_t idx = (size_t)(i - 1);   /* array index for a(i) */
        if (i <= 10) {
            cache[idx] = init[i - 1];
        } else {
            /* recurrence: a(i) = 9*a(i-1) - 8*a(i-2) - 72*a(i-3) + 96*a(i-4) + 128*a(i-5) - 128*a(i-6) - 64*a(i-7) */
            seq_t r = (seq_t)9  * cache[idx - 1];   /* a(i-1) */
            r -= (seq_t)8  * cache[idx - 2];       /* a(i-2) */
            r -= (seq_t)72 * cache[idx - 3];       /* a(i-3) */
            r += (seq_t)96 * cache[idx - 4];       /* a(i-4) */
            r += (seq_t)128 * cache[idx - 5];      /* a(i-5) */
            r -= (seq_t)128 * cache[idx - 6];      /* a(i-6) */
            r -= (seq_t)64 * cache[idx - 7];       /* a(i-7) */
            cache[idx] = r;
        }
    }

    cache_len = new_len;
}

/* sequence function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)          /* before the OEIS offset – return 0 (unused) */
        return (seq_t)0;
    extend_cache(n);
    return cache[(size_t)(n - 1)];
}
