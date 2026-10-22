/*
 * A364858  a(n) = Sum_{d|n, d < n, d in S} d, where S is the set defined in
 *          A118372.
 * https://oeis.org/A364858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d < n, d in S} d, where S is the set A118372
 *            (numbers n with a(n) <= n). The sequence a(1)=0, a(2)=1, a(3)=1,
 *            …
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Internal cache for a(n) and membership in S */
static seq_t *a_cache = NULL;
static unsigned char *s_cache = NULL;
static size_t cache_size = 0;    /* current allocated size (bytes) */
static size_t max_computed = 0;  /* highest n for which a(n) has been computed */

/* Ensure the cache can hold values up to (and including) n */
static void ensure_cache_size(idx_t n) {
    if ((size_t)(n + 1) <= cache_size) return;
    size_t new_size = cache_size ? cache_size * 2 : 1;
    while (new_size <= (size_t)(n + 1)) new_size <<= 1;
    seq_t   *new_a = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
    unsigned char *new_s = (unsigned char*)realloc(s_cache, new_size * sizeof(unsigned char));
    if (!new_a || !new_s) abort();          /* allocation failure */
    a_cache = new_a;
    s_cache = new_s;
    /* initialize the newly allocated part of s_cache to zero */
    if (new_size > cache_size) {
        memset(s_cache + cache_size, 0, new_size - cache_size);
    }
    cache_size = new_size;
}

/* Compute a(n) for all n up to the requested value */
static void compute_up_to(idx_t n) {
    if (n <= max_computed) return;
    ensure_cache_size(n);
    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        seq_t sum = 0;
        /* iterate proper divisors d of cur up to sqrt(cur) */
        for (idx_t d = 1; d <= cur / d; ++d) {
            if (cur % d == 0) {
                idx_t e = cur / d;
                if (d < cur && s_cache[d]) sum += (seq_t)d;
                if (e != d && e < cur && s_cache[e]) sum += (seq_t)e;
            }
        }
        a_cache[cur] = sum;
        s_cache[cur] = (sum <= (seq_t)cur);   /* n is in S iff a(n) <= n */
    }
    max_computed = n;
}

/* Public interface required by the build */
seq_t a(idx_t n) {
    if (n < 1) return 0;     /* undefined for n=0 in the OEIS offset, but safe */
    compute_up_to(n);
    return a_cache[n];
}
