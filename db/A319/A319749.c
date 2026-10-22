/*
 * A319749  a(n) is the numerator of the Heron sequence with h(0)=3.
 * https://oeis.org/A319749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=3;  let b(0)=1.
   For n≥0:
       a(n+1)= (a(n)^2 + 13*b(n)^2) / 2
       b(n+1)= a(n)*b(n)
   (a(n) is the numerator of the Heron sequence h(n) with h(0)=3.) */

#ifdef __cplusplus
# include <vector>
#else
# include <stdlib.h>
#endif

static size_t computed_to = 0;                         /* highest n computed so far */

#ifdef __cplusplus
/* In C++ use a vector for dynamic growth */
static std::vector<seq_t> a_cache;
static std::vector<seq_t> b_cache;

static void ensure_init(size_t n_needed) {
    if (!a_cache.size()) {
        a_cache.resize(1);
        b_cache.resize(1);
        a_cache[0] = 3;
        b_cache[0] = 1;
    }
    if (a_cache.size() <= n_needed) {
        a_cache.resize(n_needed + 1);
        b_cache.resize(n_needed + 1);
    }
}
#else
/* In C use malloc/realloc because seq_t is an unsigned integer type */
static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static size_t cache_capacity = 0;

static void ensure_init(size_t n_needed) {
    if (!a_cache) {
        cache_capacity = 1;
        a_cache = (seq_t*)malloc(sizeof(seq_t) * cache_capacity);
        b_cache = (seq_t*)malloc(sizeof(seq_t) * cache_capacity);
        a_cache[0] = 3;
        b_cache[0] = 1;
    }
    if (n_needed >= cache_capacity) {
        while (cache_capacity <= n_needed) cache_capacity <<= 1;
        a_cache = (seq_t*)realloc(a_cache, sizeof(seq_t) * cache_capacity);
        b_cache = (seq_t*)realloc(b_cache, sizeof(seq_t) * cache_capacity);
    }
}
#endif

seq_t a(idx_t n) {
    size_t idx = (size_t)n;
    ensure_init(idx);
    /* Compute missing terms up to idx */
    for (size_t i = computed_to; i < idx; ++i) {
        seq_t ai   = a_cache[i];
        seq_t bi   = b_cache[i];
        seq_t ai1  = (ai * ai + 13 * bi * bi) / 2;   /* a(i+1) */
        seq_t bi1  = ai * bi;                        /* b(i+1) */
        a_cache[i + 1] = ai1;
        b_cache[i + 1] = bi1;
    }
    computed_to = idx;
    return a_cache[idx];
}
