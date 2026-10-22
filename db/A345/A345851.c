/*
 * A345851  Numbers that are the sum of nine fourth powers in exactly nine
 *          ways.
 * https://oeis.org/A345851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1140, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A345851: numbers that are the sum of nine fourth powers in exactly nine ways.
   Uses a dynamic‑programming sieve over the set of fourth powers to count the number of
   partitions of each integer into exactly nine parts.  The array of found terms is cached
   and extended lazily until the requested index has been reached. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cached sequence values */
static seq_t *terms = NULL;
static size_t terms_len = 0;   /* number of cached terms   */
static size_t terms_cap = 0;   /* capacity of the array    */

/* Current bound processed by the DP and the last sum scanned */
static size_t max_sum = 0;
static size_t last_scanned = 0;   /* last sum examined by compute_up_to */

/* Append a new term to the cache, growing the array if necessary */
static void add_term(seq_t val)
{
    if (terms_len >= terms_cap) {
        terms_cap = terms_cap == 0 ? 128 : terms_cap << 1;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = val;
}

/* Extend the cache up to new_max_sum.  The DP counts partitions of every integer
   into exactly nine fourth powers (positive only).  Whenever a sum has exactly nine
   partitions we store the integer. */
static void compute_up_to(size_t new_max_sum)
{
    /* Generate all fourth powers <= new_max_sum (positive only). */
    uint32_t powers[2048];
    size_t powers_cnt = 0;
    for (uint64_t m = 1; ; ++m) {
        uint64_t p = m * m * m * m;
        if (p > new_max_sum) break;
        powers[powers_cnt++] = (uint32_t)p;
    }

    /* DP array: dp[k][s] = number of partitions of s into k parts.
       We only keep values up to 10 (to detect exactly 9).              */
    size_t stride = new_max_sum + 1;
    size_t dp_size = stride * 10;
    uint8_t *dp = (uint8_t *)calloc(dp_size, sizeof(uint8_t));
    if (!dp) return;   /* allocation failure – abort */

    dp[0] = 1;  /* dp[0][0] = 1 */

    for (size_t pi = 0; pi < powers_cnt; ++pi) {
        uint32_t v = powers[pi];
        for (size_t k = 1; k <= 9; ++k) {
            uint8_t *row_k   = dp + k * stride;
            uint8_t *row_prev = dp + (k - 1) * stride;
            for (size_t sum = v; sum <= new_max_sum; ++sum) {
                uint8_t add = row_prev[sum - v];
                if (add) {
                    uint8_t val = row_k[sum] + add;
                    if (val > 10) val = 10;   /* cap counts above 9 */
                    row_k[sum] = val;
                }
            }
        }
    }

    /* Scan new sums and cache those with exactly nine partitions. */
    uint8_t *row9 = dp + 9 * stride;
    for (size_t s = last_scanned + 1; s <= new_max_sum; ++s) {
        if (row9[s] == 9)
            add_term((seq_t)s);
    }

    last_scanned = new_max_sum;
    free(dp);
}

/* Return the nth term (n >= 1).  The function memoises results. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety for out-of‑range calls */

    /* Ensure the cache array is allocated. */
    if (!terms) {
        terms_cap = 128;
        terms = (seq_t *)calloc(terms_cap, sizeof(seq_t));
    }

    size_t required = (size_t)n;
    while (terms_len < required) {
        if (max_sum == 0) max_sum = 20000;   /* initial bound */
        else max_sum <<= 1;                  /* double until enough terms */

        compute_up_to(max_sum);
    }

    return terms[n - 1];
}
