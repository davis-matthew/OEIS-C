/*
 * A385107  a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) + a(n-5) with a(1)
 *          = 1, a(2) = 2, a(3) = 4, a(4) = 8, and a(5) = 15.
 * https://oeis.org/A385107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A385107
   Recurrence: a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) + a(n-5)
   Initial values: a(1)=1, a(2)=2, a(3)=4, a(4)=8, a(5)=15
   Indexing is 1‑based; a(0) is defined as 0 for convenience. */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <vector>
/* C++ implementation using std::vector to handle arbitrary‑precision types */
static std::vector<seq_t> cache;
static size_t computed = 0;

static void init_cache(void)
{
    cache.resize(6);          // indices 0..5
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)4;
    cache[4] = (seq_t)8;
    cache[5] = (seq_t)15;
    computed = 5;
}

static void ensure_capacity(size_t n)
{
    if (cache.size() <= n) cache.resize(n + 1);  // index n will be valid
}

static void compute_up_to(idx_t n)
{
    if (cache.empty()) init_cache();
    if ((size_t)n <= computed) return;

    ensure_capacity((size_t)n);

    for (idx_t i = (idx_t)(computed + 1); i <= n; ++i) {
        seq_t val = 4 * cache[(size_t)i - 1];
        val -= 6 * cache[(size_t)i - 2];
        val += 4 * cache[(size_t)i - 3];
        val -= cache[(size_t)i - 4];
        val += cache[(size_t)i - 5];
        cache[(size_t)i] = val;
    }
    computed = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    compute_up_to(n);
    return cache[(size_t)n];
}

#else
/* C implementation using malloc and placement new (when seq_t is a class) */
static seq_t *cache = NULL;
static size_t capacity = 0;   /* current allocated size - 1 (max index) */
static size_t computed  = 0;   /* highest index computed */

static void init_cache(void)
{
    capacity = 5;
    cache = (seq_t *)malloc((capacity + 1) * sizeof(seq_t));
    if (!cache) abort();   /* out of memory */

    /* If seq_t is a class with non‑trivial constructor, use placement new */
#ifdef __cplusplus
    for (size_t i = 0; i <= capacity; ++i) {
        new(&cache[i]) seq_t();
    }
#endif

    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)4;
    cache[4] = (seq_t)8;
    cache[5] = (seq_t)15;
    computed = 5;
}

static void ensure_capacity(size_t n)
{
    if (n <= capacity) return;
    size_t newcap = capacity ? capacity : 1;
    while (newcap < n) newcap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, (newcap + 1) * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */

    /* If seq_t is a class, construct the new elements */
#ifdef __cplusplus
    for (size_t i = capacity + 1; i <= newcap; ++i) {
        new(&tmp[i]) seq_t();
    }
#endif

    cache = tmp;
    capacity = newcap;
}

static void compute_up_to(idx_t n)
{
    if (!cache) init_cache();
    if ((size_t)n <= computed) return;

    ensure_capacity((size_t)n);

    for (idx_t i = (idx_t)(computed + 1); i <= n; ++i) {
        seq_t val = 4 * cache[(size_t)i - 1];
        val -= 6 * cache[(size_t)i - 2];
        val += 4 * cache[(size_t)i - 3];
        val -= cache[(size_t)i - 4];
        val += cache[(size_t)i - 5];
        cache[(size_t)i] = val;
    }
    computed = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    compute_up_to(n);
    return cache[(size_t)n];
}

#endif
