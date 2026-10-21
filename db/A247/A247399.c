/*
 * A247399  Number of length n+4 0..3 arrays with no disjoint pairs in any
 *          consecutive five terms having the same sum.
 * https://oeis.org/A247399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-5) - 8*a(n-10) (for n > 10) Initial values (offset
 *            1): a(1) = 200, a(2) = 264, a(3) = 340, a(4) = 428, a(5) = 528,
 *            a(6) = 640, a(7) = 864, a(8) = 1136, a(9) = 1456, a(10) = 1824
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 63, u64 n <= 143, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Helper to adapt the standard realloc approach to this build */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap == 0 ? 16 : *cap * 2);
    while (new_cap < needed)
        new_cap *= 2;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) {
        /* In a real system we would handle out‑of‑memory; here we abort */
        abort();
    }
    *ptr = tmp;
    *cap = new_cap;
}

/* Cached sequence values */
static seq_t *a_cache = NULL;      /* 0‑based: a_cache[0] == a(1) */
static size_t a_cap   = 0;          /* allocated capacity */
static size_t a_len   = 0;          /* number of values currently computed */

/* Pre‑defined first ten values */
static const seq_t init_vals[10] = {
    (seq_t)200, (seq_t)264, (seq_t)340, (seq_t)428,
    (seq_t)528, (seq_t)640, (seq_t)864, (seq_t)1136,
    (seq_t)1456, (seq_t)1824
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1, value for 0 is undefined; return sentinel */

    /* Ensure we have space for n values (1‑based) */
    ensure_capacity((size_t)n, &a_cap,
                    (void **)&a_cache, sizeof(seq_t));

    /* Fill first ten values if not already done */
    if (a_len == 0) {
        for (int i = 0; i < 10; ++i)
            a_cache[i] = init_vals[i];
        a_len = 10;
    }

    /* If the requested index is already computed, just return it */
    if (n <= a_len)
        return a_cache[(size_t)n - 1];

    /* Compute values from a_len up to n-1 (0‑based) */
    for (size_t i = a_len; i < (size_t)n; ++i) {
        /* i corresponds to a(i+1) */
        seq_t val1 = a_cache[i - 5];
        seq_t val2 = a_cache[i - 10];
        a_cache[i] = (seq_t)6 * val1 - (seq_t)8 * val2;
    }
    a_len = n;
    return a_cache[(size_t)n - 1];
}
