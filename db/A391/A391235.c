/*
 * A391235  Number of integer compositions of n that are the first sums of
 *          some composition with all parts > 1.
 * https://oeis.org/A391235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 46, u32 n <= 91, u64 n <= 183,
 *            u128 n <= 368, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-2) + a(n-5) – a(n-6) – a(n-7) – a(n-9)
   for n ≥ 10, with offset 1 and initial values
   a(1)=0,a(2)=0,a(3)=0,a(4)=1,a(5)=1,a(6)=1,a(7)=1,a(8)=2,a(9)=3. */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;     /* array of computed a(n) (index 0 unused) */
static size_t cache_len = 0;    /* number of valid entries (index 0 … cache_len-1) */
static size_t cache_cap = 0;    /* capacity of cache array */

/* initialise cache with the first 10 terms (indices 0‑9) */
static void init_cache(void)
{
    cache_cap = 10;
    cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    if (!cache) abort();

    cache_len = 10;
    /* a(0) is unused in the sequence but kept for convenience */
    cache[0] = (seq_t)0;          /* unused */
    cache[1] = (seq_t)0;          /* a(1) */
    cache[2] = (seq_t)0;          /* a(2) */
    cache[3] = (seq_t)0;          /* a(3) */
    cache[4] = (seq_t)1;          /* a(4) */
    cache[5] = (seq_t)1;          /* a(5) */
    cache[6] = (seq_t)1;          /* a(6) */
    cache[7] = (seq_t)1;          /* a(7) */
    cache[8] = (seq_t)2;          /* a(8) */
    cache[9] = (seq_t)3;          /* a(9) */
}

/* enlarge cache capacity until it can hold indices up to n */
static void enlarge_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= (size_t)n + 1)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* extend cache to include value at index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (!cache) init_cache();
    if (n < (idx_t)cache_len)
        return;

    enlarge_cache(n);

    /* compute missing values */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i < 10) {
            /* initial values already set */
            ++cache_len;
            continue;
        }
        seq_t val = 2 * cache[i - 2] + cache[i - 5];
        val -= cache[i - 6];
        val -= cache[i - 7];
        val -= cache[i - 9];
        cache[i] = val;
        ++cache_len;
    }
}

/* public function required by the specification */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1, but harmless */

    ensure_cache(n);
    return cache[n];
}
