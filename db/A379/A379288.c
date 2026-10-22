/*
 * A379288  Irregular triangle read by rows in which row n lists the odd
 *          divisors of n excluding odd divisors e for which there exists
 *          another divisor j with j < e < 2*j.
 * https://oeis.org/A379288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th entry read row‑by‑row of the irregular
 *            triangle. Row m contains the odd divisors of m that are not
 *            excluded by the rule “there exists a divisor j of m with j < e <
 *            2·j”. The sequence is built incrementally by appending each row
 *            to a growing cache
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache of the sequence values   */
static size_t cache_len = 0;          /* number of values stored       */
static size_t cache_cap = 0;          /* current capacity of cache     */
static idx_t  current_row = 0;        /* last row that has been processed */

/* comparison function for qsort to sort divisor array (ascending) */
static int cmp_uint64(const void *a, const void *b) {
    const uint64_t *va = (const uint64_t *)a;
    const uint64_t *vb = (const uint64_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return 1;
    return 0;
}

/* enlarge the cache when needed */
static void grow_cache(void) {
    size_t new_cap = (cache_cap == 0) ? 128 : cache_cap << 1;
    seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!ptr) abort();                 /* allocation failure */
    cache = ptr;
    cache_cap = new_cap;
}

/* append a value to the cache, growing it if necessary */
static void append_to_cache(seq_t val) {
    if (cache_len >= cache_cap) grow_cache();
    cache[cache_len++] = val;
}

/* generate the row for the given integer m and append its qualifying odd divisors */
static void generate_row(idx_t m) {
    uint64_t M = (uint64_t)m;
    uint64_t divs[256];
    size_t dcnt = 0;

    /* collect all divisors of M */
    for (uint64_t i = 1; i <= M / i; ++i) {
        if (M % i == 0) {
            divs[dcnt++] = i;
            if (i != M / i) divs[dcnt++] = M / i;
        }
    }
    /* sort the divisor list */
    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    /* process odd divisors and apply the exclusion rule */
    for (size_t idx = 0; idx < dcnt; ++idx) {
        uint64_t d = divs[idx];
        if ((d & 1ULL) == 0) continue;              /* only odd divisors */
        int include = 1;
        if (idx > 0) {
            uint64_t prev = divs[idx - 1];            /* largest divisor < d */
            if (prev > d / 2) include = 0;          /* excluded by rule   */
        }
        if (include) append_to_cache((seq_t)d);
    }
}

/* make sure the cache contains at least n terms */
static void ensure_cache(idx_t n) {
    while (cache_len < (size_t)n) {
        ++current_row;               /* next row index (starting from 1) */
        generate_row(current_row);
    }
}

/* required function; returns the n-th term of the sequence */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;     /* undefined for n==0, return 0 as safe default */
    if (cache_len < (size_t)n) ensure_cache(n);
    return cache[(size_t)n - 1];
}
