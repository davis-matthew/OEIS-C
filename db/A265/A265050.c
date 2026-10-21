/*
 * A265050  Poincaré series for hyperbolic reflection group with Coxeter
 *          diagram shown in Comments.
 * https://oeis.org/A265050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 45, u64 n <= 95, u128 n <= 195,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A265050:  G.f. = (1+x)^3(1-x+x^2)(1+x^2) /
                        ((1-x)(1-x-x^2+x^3-x^4-x^5+x^6))
   Recurrence for n ≥ 7:
      a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - 2*a(n-6) + a(n-7) + b(n)
   where b(n) = coefficient of x^n in the numerator:
      b = {1,2,2,3,3,2,2,1}  (b(n)=0 for n≥8)
   The first eight values are taken from the sequence data:
      a(0)=1,a(1)=4,a(2)=10,a(3)=21,a(4)=39,a(5)=68,a(6)=114,a(7)=186
*/

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size */
static size_t cache_len = 0;   /* number of computed entries (cache[0..len-1]) */

/* initialise the cache with the first eight terms */
static void init_cache(void)
{
    if (cache != NULL) return;
    cache = (seq_t*)malloc(8 * sizeof(seq_t));
    if (!cache) return;   /* memory allocation failed */
    cache_cap = 8;
    /* seed the first eight a(n) */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)10;
    cache[3] = (seq_t)21;
    cache[4] = (seq_t)39;
    cache[5] = (seq_t)68;
    cache[6] = (seq_t)114;
    cache[7] = (seq_t)186;
    cache_len = 8;
}

/* enlarge cache to hold at least up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    init_cache();
    if ((size_t)n < cache_len) return;

    /* increase capacity */
    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) return;   /* allocation failed – leave cache unchanged */
    cache = new_cache;
    cache_cap = new_cap;

    /* recurrence coefficients and numerator coefficients */
    static const seq_t b[8] = {1,2,2,3,3,2,2,1};

    /* fill missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t bi = (i <= 7) ? b[(size_t)i] : (seq_t)0;
        seq_t val = 0;
        /* a(i-1) term: 2*a(i-1) */
        val += (seq_t)2 * cache[i-1];
        /* -2*a(i-3) term */
        val -= (seq_t)2 * cache[i-3];
        /* +2*a(i-4) term */
        val += (seq_t)2 * cache[i-4];
        /* -2*a(i-6) term */
        val -= (seq_t)2 * cache[i-6];
        /* +1*a(i-7) term */
        val += cache[i-7];
        /* +b(i) term */
        val += bi;
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
