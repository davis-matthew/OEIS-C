/*
 * A250820  Number of (n+2) X (1+2) 0..1 arrays with nondecreasing maximum
 *          minus minimum of every three consecutive values in every row and
 *          column.
 * https://oeis.org/A250820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) -8*a(n-2) -27*a(n-3) +45*a(n-4) +24*a(n-5)
 *            -51*a(n-6) -3*a(n-7) +16*a(n-8) -a(n-9) -a(n-10), a(1)=512,
 *            a(2)=2744, a(3)=13824, a(4)=64000, a(5)=287496, a(6)=1259712,
 *            a(7)=5451776, a(8)=23393656, a(9)=99897344, a(10)=425259008
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 26, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Initial ten terms of the sequence. */
static const seq_t init_terms[10] = {
    (seq_t)512ULL,
    (seq_t)2744ULL,
    (seq_t)13824ULL,
    (seq_t)64000ULL,
    (seq_t)287496ULL,
    (seq_t)1259712ULL,
    (seq_t)5451776ULL,
    (seq_t)23393656ULL,
    (seq_t)99897344ULL,
    (seq_t)425259008ULL
};

/* Cache for computed terms: cache[i] holds a(i), i >= 1. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated size, indices 0..cache_cap-1 */
static size_t last_computed = 0; /* largest i for which cache[i] is defined */

/* Ensure the cache can store values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = (size_t)n + 1;
    if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
    if (new_cap == 0) new_cap = 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(k) for all k <= n that have not yet been computed. */
static void compute_up_to(idx_t n)
{
    if (n < 1) return;
    ensure_cache(n);

    /* Initialise the first ten terms if not already present. */
    if (last_computed < 10) {
        size_t up = (10 < (size_t)n) ? 10 : (size_t)n;
        for (size_t i = 1; i <= up; ++i) cache[i] = init_terms[i - 1];
        last_computed = up;
    }

    /* Compute remaining terms using the recurrence. */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        /* Separate positive and negative contributions to avoid unsigned underflow. */
        seq_t pos = 0;
        seq_t neg = 0;

        /* Positive coefficients */
        pos += ((seq_t)7)  * cache[i - 1];
        pos += ((seq_t)45) * cache[i - 4];
        pos += ((seq_t)24) * cache[i - 5];
        pos += ((seq_t)16) * cache[i - 8];

        /* Negative coefficients (sum of absolute values) */
        neg += ((seq_t)8)  * cache[i - 2];
        neg += ((seq_t)27) * cache[i - 3];
        neg += ((seq_t)51) * cache[i - 6];
        neg += ((seq_t)3)  * cache[i - 7];
        neg += ((seq_t)1)  * cache[i - 9];
        neg += ((seq_t)1)  * cache[i - 10];

        cache[i] = pos - neg;
    }
    last_computed = (size_t)n;
}

/* Public function to obtain the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;              /* undefined for n < 1 */
    compute_up_to(n);
    return cache[(size_t)n];
}
