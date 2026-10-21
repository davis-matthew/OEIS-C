/*
 * A283411  Number of nX4 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A283411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 8*a(n‑1) +58*a(n‑2)+137*a(n‑3)+127*a(n‑4)+60*a(n‑5)
 *             -128*a(n‑6)+83*a(n‑7)-150*a(n‑8)+97*a(n‑9)-59*a(n‑10)
 *             +32*a(n‑11)-9*a(n‑12)+a(n‑13)   for n>16
 * Initial values:
 * a(1)=16, a(2)=236, a(3)=2944, a(4)=39268, a(5)=519252,
 * a(6)=6865368, a(7)=90803472, a(8)=1200890168,
 * a(9)=15882218184, a(10)=210047690772, a(11)=2777952473964,
 * a(12)=36739369719812, a(13)=485890702739492,
 * a(14)=6426070385501460, a(15)=84986974086126936,
 * a(16)=1123981738831575460.
 */
#include <stdlib.h>

/* pre‑computed first 16 terms */
static const seq_t init[16] = {
    (seq_t)16,
    (seq_t)236,
    (seq_t)2944,
    (seq_t)39268,
    (seq_t)519252,
    (seq_t)6865368,
    (seq_t)90803472,
    (seq_t)1200890168,
    (seq_t)15882218184,
    (seq_t)210047690772,
    (seq_t)2777952473964,
    (seq_t)36739369719812,
    (seq_t)485890702739492,
    (seq_t)6426070385501460,
    (seq_t)84986974086126936,
    (seq_t)1123981738831575460
};

static seq_t *cache = NULL;   /* 0‑based: cache[0] = a(1) */
static size_t cache_cap = 0;  /* allocated number of elements */
static idx_t  cache_max = 0;  /* largest n whose value is stored */

/* ensure cache can hold at least n elements (1‑based) */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
    if (cache_max == 0) {
        /* first allocation: store the initial 16 values */
        size_t i;
        for (i = 0; i < 16 && i < cache_cap; ++i)
            cache[i] = init[i];
        cache_max = i;   /* cache_max becomes 16 (or less if cap<16) */
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */
    ensure_cache_size(n);
    if (n <= cache_max)                    /* value already known */
        return cache[(size_t)(n-1)];

    /* compute terms from cache_max+1 up to n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t val;
        if (k <= 16) {
            val = init[(size_t)(k-1)];    /* initial values are already valid */
        } else {
            val = 0;
            /* positive contributions */
            val += (seq_t)8   * cache[(size_t)(k-2)];   /* a(k-1) */
            val += (seq_t)58  * cache[(size_t)(k-3)];   /* a(k-2) */
            val += (seq_t)137 * cache[(size_t)(k-4)];   /* a(k-3) */
            val += (seq_t)127 * cache[(size_t)(k-5)];   /* a(k-4) */
            val += (seq_t)60  * cache[(size_t)(k-6)];   /* a(k-5) */
            val -= (seq_t)128 * cache[(size_t)(k-7)];   /* a(k-6) */
            val += (seq_t)83  * cache[(size_t)(k-8)];   /* a(k-7) */
            val -= (seq_t)150 * cache[(size_t)(k-9)];   /* a(k-8) */
            val += (seq_t)97  * cache[(size_t)(k-10)];  /* a(k-9) */
            val -= (seq_t)59  * cache[(size_t)(k-11)];  /* a(k-10) */
            val += (seq_t)32  * cache[(size_t)(k-12)];  /* a(k-11) */
            val -= (seq_t)9   * cache[(size_t)(k-13)];  /* a(k-12) */
            val += cache[(size_t)(k-14)];              /* a(k-13) */
        }
        cache[(size_t)(k-1)] = val;
    }
    cache_max = n;
    return cache[(size_t)(n-1)];
}
