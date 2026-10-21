/*
 * A260494  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 00000011 or 00001011.
 * https://oeis.org/A260494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A260494:  a(n) = 4*a(n-3)+4*a(n-4)+4*a(n-5)+4*a(n-6)+4*a(n-7)
 *          +3*a(n-8)+2*a(n-9)+3*a(n-10)+2*a(n-11)+a(n-12) for n>14.
 *  Initial values:
 *  a(1)=48, a(2)=83, a(3)=218, a(4)=464, a(5)=722, a(6)=1600,
 *  a(7)=3446, a(8)=6277, a(9)=12596, a(10)=26271, a(11)=51082,
 *  a(12)=100763, a(13)=204716, a(14)=406841
 *
 * The function is memoised and works for any order of calls.
 * It uses only 64‑bit arithmetic for indices and allocates on the heap once.
 */

#include <stdlib.h>

/* internal storage for memoisation */
static seq_t *cache = NULL;     /* 1‑based index, cache[0] unused */
static idx_t  cache_cap = 0;    /* allocated capacity (>= max index + 1) */
static idx_t  last_computed = 0;/* greatest index whose value is stored */

/* ensure memory for indices up to 'need' is allocated */
static void ensure_cache(idx_t need)
{
    if (cache_cap > need) return;          /* already enough */
    idx_t new_cap = need + 1;               /* include index 'need' */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                     /* out of memory */
    /* initialise new cells to zero */
    for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
    cache = tmp;
    cache_cap = new_cap;
}

/* initialise the first 14 terms once */
static void init_initial_terms(void)
{
    /* cache[0] is unused */
    cache[1] = (seq_t)48;
    cache[2] = (seq_t)83;
    cache[3] = (seq_t)218;
    cache[4] = (seq_t)464;
    cache[5] = (seq_t)722;
    cache[6] = (seq_t)1600;
    cache[7] = (seq_t)3446;
    cache[8] = (seq_t)6277;
    cache[9] = (seq_t)12596;
    cache[10] = (seq_t)26271;
    cache[11] = (seq_t)51082;
    cache[12] = (seq_t)100763;
    cache[13] = (seq_t)204716;
    cache[14] = (seq_t)406841;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1, behaviour below it is irrelevant */

    /* make sure we have space for at least n and the initial 14 terms */
    idx_t need = (n > 14) ? n : 14;
    ensure_cache(need);

    /* initialise the first 14 terms only once */
    if (last_computed < 14) {
        init_initial_terms();
        last_computed = 14;
    }

    if (n <= 14) return cache[n];  /* already known */

    /* compute further values iteratively, using memoisation */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)0;
        val += (seq_t)4 * cache[i-3];
        val += (seq_t)4 * cache[i-4];
        val += (seq_t)4 * cache[i-5];
        val += (seq_t)4 * cache[i-6];
        val += (seq_t)4 * cache[i-7];
        val += (seq_t)3 * cache[i-8];
        val += (seq_t)2 * cache[i-9];
        val += (seq_t)3 * cache[i-10];
        val += (seq_t)2 * cache[i-11];
        val +=          cache[i-12];
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
