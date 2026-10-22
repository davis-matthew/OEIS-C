/*
 * A393344  Number of Cayley permutations of size n avoiding 123 and 231.
 * https://oeis.org/A393344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(3)=11, a(4)=41, a(5)=145, a(6)=483 For
 *            n≥7, a(n) satisfies the linear recurrence a(n) = 11*a(n-1) -
 *            49*a(n-2) + 113*a(n-3) - 142*a(n-4) + 92*a(n-5) - 24*a(n-6) The
 *            recurrence is derived from the generating function of A393344
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array of computed terms */
static size_t cache_size = 0;        /* allocated capacity of the array      */
static idx_t last_computed = 0;      /* highest index that has been filled   */

/* Initialize cache with the first 7 terms of the sequence.  */
static void init_cache(void)
{
    size_t sz = 8; /* at least enough for indices 0..7                         */
    cache = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!cache) abort();
    cache_size = sz;
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)11;
    cache[4] = (seq_t)41;
    cache[5] = (seq_t)145;
    cache[6] = (seq_t)483;
    last_computed = 6;
}

/* Grow cache so that it can hold at least index n.  */
static void ensure_cache(idx_t n)
{
    if (!cache) {
        init_cache();
        return;
    }
    /* Grow capacity if necessary (doubling growth). */
    if ((size_t)n >= cache_size) {
        size_t new_size = cache_size ? cache_size * 2 : 8;
        while (new_size <= (size_t)n) new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }
}

/* Public sequence function.  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if ((idx_t)n <= last_computed)
        return cache[n];

    /* Compute terms from last_computed+1 up to n using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t t5 = cache[i - 5];
        seq_t t6 = cache[i - 6];

        seq_t pos = (seq_t)(11) * t1 + (seq_t)(113) * t3 + (seq_t)(92) * t5;
        seq_t neg = (seq_t)(49) * t2 + (seq_t)(142) * t4 + (seq_t)(24) * t6;

        cache[i] = pos - neg;          /* pos is always > neg for n≥7 */
    }
    last_computed = n;
    return cache[n];
}
