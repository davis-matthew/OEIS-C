/*
 * A260835  Number of (n+2)X(2+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 00000011 or 00010101.
 * https://oeis.org/A260835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 30, u64 n <= 67, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A260835
 *  Formula (recurrence): 
 *    a(1)=82, a(2)=131, a(3)=288, a(4)=496, a(5)=796,
 *    a(6)=1645, a(7)=2976, a(8)=4890, a(9)=9332, a(10)=17659,
 *    a(11)=30090, a(12)=54846, a(13)=104380, a(14)=184549,
 *    a(15)=329454, a(16)=618800, a(17)=1121728, a(18)=1998197,
 *    a(19)=3692344
 *    For n>19:
 *      a(n)=a(n-1)+5*a(n-3)-a(n-4)-3*a(n-5)-5*a(n-6)
 *           +3*a(n-9)-2*a(n-10)-5*a(n-11)-2*a(n-12)
 *           -a(n-13)+5*a(n-14)-2*a(n-15)-2*a(n-17)+2*a(n-18)
 */
#include <stdlib.h>

static seq_t *cache = NULL;        /* cached a values, cache[0]=a(1) */
static idx_t cache_cap = 0;        /* allocated capacity */
static idx_t cache_max = 0;        /* number of terms already computed */

/* first 19 terms known from the OEIS sequence */
static const seq_t base_vals[19] = {
    82ULL, 131ULL, 288ULL, 496ULL, 796ULL,
    1645ULL, 2976ULL, 4890ULL, 9332ULL, 17659ULL,
    30090ULL, 54846ULL, 104380ULL, 184549ULL, 329454ULL,
    618800ULL, 1121728ULL, 1998197ULL, 3692344ULL
};

seq_t a(idx_t n)
{
    /* behaviour for n<1 is irrelevant */
    if (n < 1) return 0;

    /* allocate or enlarge cache to hold at least n entries */
    if (cache == NULL || cache_cap < n) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return 0;   /* allocation failed; undefined result */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base terms if not yet present */
    if (cache_max < 19) {
        idx_t upto = 19;
        if (upto > n) upto = (idx_t)n;
        for (idx_t i = cache_max; i < upto; ++i)
            cache[i] = base_vals[i];
        cache_max = upto;
    }

    /* if requested n is within pre‑computed range, return */
    if (n <= cache_max)
        return cache[n - 1];

    /* compute further terms using the given recurrence */
    for (idx_t idx = cache_max + 1; idx <= n; ++idx) {
        seq_t val = 0;
        /* indices in cache are 0‑based: a(k) -> cache[k-1] */
        val += cache[idx - 2];                       /* a(n-1) */
        val += (seq_t)5 * cache[idx - 4];             /* +5*a(n-3) */
        val -= cache[idx - 5];                       /* -a(n-4) */
        val -= (seq_t)3 * cache[idx - 6];             /* -3*a(n-5) */
        val -= (seq_t)5 * cache[idx - 7];             /* -5*a(n-6) */
        val += (seq_t)3 * cache[idx - 10];            /* +3*a(n-9) */
        val -= (seq_t)2 * cache[idx - 11];            /* -2*a(n-10) */
        val -= (seq_t)5 * cache[idx - 12];            /* -5*a(n-11) */
        val -= (seq_t)2 * cache[idx - 13];            /* -2*a(n-12) */
        val -= cache[idx - 14];                       /* -a(n-13) */
        val += (seq_t)5 * cache[idx - 15];            /* +5*a(n-14) */
        val -= (seq_t)2 * cache[idx - 16];            /* -2*a(n-15) */
        val -= (seq_t)2 * cache[idx - 18];            /* -2*a(n-17) */
        val += (seq_t)2 * cache[idx - 19];            /* +2*a(n-18) */
        cache[idx - 1] = val;
    }
    cache_max = n;

    return cache[n - 1];
}
