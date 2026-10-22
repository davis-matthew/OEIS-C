/*
 * A368628  Expansion of g.f. A(x) satisfying A(x) = 1 + x*(A(x)^2 -
 *          A(-x)^2)/2 + x*(A(x)^4 + A(-x)^4)/2.
 * https://oeis.org/A368628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n >= 1: if n is even: a(n) = sum_{k=0}^{n-1}
 *            a(k)*a(n-1-k); if n is odd : a(n) = sum_{k=0}^{n-1}
 *            b(k)*b(n-1-k), where b(m) = sum_{k=0}^{m} a(k)*a(m-k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static size_t cache_cap = 0;     /* number of entries allocated */
static idx_t computed_up_to = 0; /* maximum index fully computed */

/* ensure enough space to store indices 0..n */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
    seq_t *new_a = (seq_t *)malloc(new_cap * sizeof(seq_t));
    seq_t *new_b = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (a_cache) {
        memcpy(new_a, a_cache, cache_cap * sizeof(seq_t));
        free(a_cache);
    }
    if (b_cache) {
        memcpy(new_b, b_cache, cache_cap * sizeof(seq_t));
        free(b_cache);
    }
    a_cache = new_a;
    b_cache = new_b;
    /* zero new entries */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        a_cache[i] = (seq_t)0;
        b_cache[i] = (seq_t)0;
    }
    cache_cap = new_cap;
}

/* Initialize base case if needed */
static void init_cache(void)
{
    if (a_cache) return;  /* already initialized */
    ensure_cache(0);
    a_cache[0] = (seq_t)1;  /* a(0) = 1 */
    b_cache[0] = (seq_t)1;  /* b(0) = a(0)*a(0) */
    computed_up_to = 0;     /* we have computed index 0 */
}

seq_t a(idx_t n)
{
    init_cache();
    ensure_cache(n);

    /* compute from computed_up_to+1 up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {

        if ((i & 1U) == 0U) {   /* i even */
            a_cache[i] = b_cache[i - 1];
        } else {                /* i odd */
            seq_t sum = (seq_t)0;
            /* sum_{k=0}^{i-1} b(k) * b(i-1-k) */
            for (idx_t k = 0; k <= i - 1; ++k) {
                sum += b_cache[k] * b_cache[i - 1 - k];
            }
            a_cache[i] = sum;
        }

        /* compute b(i) = sum_{k=0}^{i} a(k)*a(i-k) */
        seq_t sb = (seq_t)0;
        for (idx_t k = 0; k <= i; ++k) {
            sb += a_cache[k] * a_cache[i - k];
        }
        b_cache[i] = sb;
    }
    computed_up_to = n;
    return a_cache[n];
}
