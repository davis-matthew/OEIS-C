/*
 * A362542  Number of chordless cycles of length >=4 in the n-Goldberg graph.
 * https://oeis.org/A362542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Linear recurrence of order 14
 *
 * a(n) = 9*a(n-1)-27*a(n-2)+43*a(n-3)-82*a(n-4)+62*a(n-5)+171*a(n-6)
 *       -57*a(n-7)-231*a(n-8)-161*a(n-9)+198*a(n-10)+204*a(n-11)
 *       -52*a(n-12)-68*a(n-13)-8*a(n-14)
 *
 * Initial values (OEIS offset 1):
 * a(1)=0,  a(2)=29,  a(3)=168,  a(4)=617,  a(5)=2028,
 * a(6)=7755, a(7)=31790, a(8)=127921, a(9)=519144, a(10)=2156929,
 * a(11)=9033368, a(12)=37912247, a(13)=159883298, a(14)=677172001
 */

#include <stddef.h>
#include <stdlib.h>

static const int coeff[14] = {
    9, -27, 43, -82, 62, 171,
    -57, -231, -161, 198, 204, -52, -68, -8
};

static seq_t *cache = NULL;     /* stores a(1) to a(N) as cache[0]..[N-1] */
static size_t cache_size = 0;   /* number of elements currently stored */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* outside defined range; undefined behaviour otherwise */

    /* initialise cache with the 14 known terms on first call */
    if (cache_size == 0) {
        cache = (seq_t*)malloc(14 * sizeof(seq_t));
        if (!cache) { /* allocation failed; return 0 as sentinel */
            return (seq_t)0;
        }
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)29;
        cache[2] = (seq_t)168;
        cache[3] = (seq_t)617;
        cache[4] = (seq_t)2028;
        cache[5] = (seq_t)7755;
        cache[6] = (seq_t)31790;
        cache[7] = (seq_t)127921;
        cache[8] = (seq_t)519144;
        cache[9] = (seq_t)2156929;
        cache[10] = (seq_t)9033368;
        cache[11] = (seq_t)37912247;
        cache[12] = (seq_t)159883298;
        cache[13] = (seq_t)677172001;
        cache_size = 14;
    }

    /* if the requested index is already cached, return it */
    if (n <= cache_size) {
        return cache[n-1];
    }

    /* extend the cache to accommodate a(n) */
    seq_t *new_cache = (seq_t*)realloc(cache, n * sizeof(seq_t));
    if (!new_cache) {
        /* realloc failed; leave the cache unchanged and return 0 */
        return 0;
    }
    cache = new_cache;

    /* compute new terms using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int j = 1; j <= 14; ++j) {
            int c = coeff[j-1];
            /* index inside cache array: a(i-j) -> cache[(i-j)-1] = i-j-1 */
            seq_t term = cache[(size_t)(i - j - 1)];
            seq_t prod = (seq_t)((c >= 0) ? c : -c) * term;
            if (c >= 0) {
                pos += prod;
            } else {
                neg += prod;
            }
        }
        /* recurrence ensures pos >= neg for all n */
        cache[i-1] = pos - neg;
    }
    cache_size = (size_t)n;
    return cache[n-1];
}
