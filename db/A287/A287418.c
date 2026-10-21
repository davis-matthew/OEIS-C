/*
 * A287418  Start with 0 and repeatedly substitute 0->012, 1->120, 2->102.
 * https://oeis.org/A287418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A287418: the fixed point of the morphism
   0 → 012, 1 → 120, 2 → 102, starting from a single 0.
   a(n) is the (n‑1)-th symbol of the infinite word (n ≥ 1). */

#if defined(__cplusplus)
#include <new>
#else
#include <stdlib.h>
#endif

/* portable allocation / deallocation for both C and C++ */
#if defined(__cplusplus)
static inline seq_t *alloc_seq_arr(size_t n) { return new seq_t[n]; }
static inline void    free_seq_arr(seq_t *p) { delete [] p; }
#else
static inline seq_t *alloc_seq_arr(size_t n) { return malloc(n * sizeof(seq_t)); }
static inline void    free_seq_arr(seq_t *p) { free(p); }
#endif

/* cache of already generated symbols */
static seq_t *cache = NULL;
static idx_t  cached_len = 0;

/* mapping according to the morphism: 0→012, 1→120, 2→102 */
static const seq_t next_map[3][3] = {
    { 0, 1, 2 },   /* 0 -> 012 */
    { 1, 2, 0 },   /* 1 -> 120 */
    { 1, 0, 2 }    /* 2 -> 102 */
};

/* grow the cache until it contains at least n symbols (n ≥ 1) */
static void ensure_cache(idx_t n)
{
    /* first generation: start with a single 0 */
    if (!cache) {
        cache = alloc_seq_arr(1);
        if (!cache) return;          /* allocation failure – undefined */
        cache[0] = 0;
        cached_len = 1;
        return;
    }

    /* repeated application of the morphism */
    while (cached_len < n) {
        idx_t new_len = cached_len * 3;
        seq_t *new_cache = alloc_seq_arr(new_len);
        if (!new_cache) return;      /* allocation failure – undefined */

        for (idx_t i = 0; i < cached_len; ++i) {
            unsigned int v = (unsigned int)cache[i]; /* v is 0,1,2 */
            new_cache[i*3 + 0] = next_map[v][0];
            new_cache[i*3 + 1] = next_map[v][1];
            new_cache[i*3 + 2] = next_map[v][2];
        }

        free_seq_arr(cache);
        cache = new_cache;
        cached_len = new_len;
    }
}

/* seq_t a(idx_t n) – required external linkage function */
seq_t a(idx_t n)
{
    if (n == 0)                /* below the OEIS offset, return 0 */
        return 0;
    ensure_cache(n);
    return cache[n-1];
}
