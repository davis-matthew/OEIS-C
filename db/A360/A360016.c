/*
 * A360016  Number of partitions of 4*n into four odd primes (p_1, p_2, p_3,
 *          p_4) (p_1 < p_2 <= p_3 < p_4 and p_1 + p_4 = p_2 + p_3 = 2*n) such
 *          that (p_1, p_2) and (p_3, p_4) are consecutive pairs of prime
 *          numbers with the same difference, d = p_2 - p_1 = p_4 - p_3, and
 *          (p_1, p_3), (p_2, p_4) are also consec...
 * https://oeis.org/A360016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lower_bound_uint16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A360016:
 *  Number of partitions of 4*n into four odd primes
 *  p1 < p2 <= p3 < p4 with
 *     p1 + p4 = p2 + p3 = 2*n
 *  such that (p1,p2) and (p3,p4) are consecutive prime pairs with
 *  the same difference d, and (p1,p3),(p2,p4) are consecutive prime
 *  pairs with the same difference D.
 *
 *  The function builds the list of primes up to 4*n and, for each even
 *  difference d, builds the vector of start primes p for which p and
 *  p+d are prime.  A partition is counted when the required
 *  consecutive‑pair constraints are satisfied.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/lower_bound_uint16.h"

/* ----- global data (per limit) ----- */
static uint8_t  *prime_flags  = NULL;   /* prime flag array          */
static uint32_t *prime_list   = NULL;   /* list of primes            */
static size_t   prime_count   = 0;      /* number of primes          */

static uint16_t **diff_vectors = NULL;  /* diff_vectors[d] -> starts */
static size_t   *diff_len      = NULL;  /* number of entries per d   */
static size_t    max_limit      = 0;     /* largest limit processed   */

/* ----- helper prototypes ----- */
static void init_structures(idx_t n);
static inline bool check_consecutive(uint32_t d, uint32_t p1, uint32_t p3);
static inline size_t lower_bound_uint16(const uint16_t *arr, size_t len, uint32_t val);

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* build data up to limit = 4*n */
    init_structures(n);

    seq_t  count = 0;
    uint32_t limit = (uint32_t)(4 * n);           /* n <= 10000  → limit <= 40000  */
    size_t  idx_n = 0;                            /* primes <= n */
    while (idx_n < prime_count && prime_list[idx_n] <= (uint32_t)n) ++idx_n;

    for (size_t i = 0; i < idx_n; ++i) {
        uint32_t p1 = prime_list[i];
        if (p1 < 3) continue;                     /* odd primes only */

        uint32_t S = (uint32_t)n - p1;            /* S = a + b  ,  a,b >= 1 */
        if (S < 2) continue;

        uint32_t max_a = S / 2;
        for (uint32_t a = 1; a <= max_a; ++a) {
            uint32_t b = S - a;                  /* b >= a              */
            uint32_t d = 2 * a;
            uint32_t D = 2 * b;

            uint32_t p2 = p1 + d;
            if (p2 > limit || !prime_flags[p2]) continue;
            uint32_t p3 = p1 + D;
            if (p3 > limit || !prime_flags[p3]) continue;
            uint32_t p4 = p3 + d;                /* equals p1 + d + D   */
            if (p4 > limit || !prime_flags[p4]) continue;

            if (!check_consecutive(d,  p1, p3)) continue;
            if (!check_consecutive(D,  p1, p2)) continue;

            ++count;
        }
    }
    return count;
}

/* ----- build primes and difference vectors for limit = 4*n ----- */
static void init_structures(idx_t n)
{
    uint32_t limit = (uint32_t)(4 * n);
    if (limit <= max_limit) return;              /* already ready */

    /* free previous data  */
    if (prime_flags) free(prime_flags);
    if (prime_list)  free(prime_list);
    if (diff_vectors) {
        for (size_t d = 0; d <= max_limit; ++d)
            if (diff_vectors[d]) free(diff_vectors[d]);
        free(diff_vectors);
        free(diff_len);
    }
    prime_flags  = NULL;
    prime_list   = NULL;
    diff_vectors = NULL;
    diff_len     = NULL;
    prime_count  = 0;
    max_limit    = 0;

    /* -------- sieve of Eratosthenes -------- */
    prime_flags = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
    if (!prime_flags) return;
    for (uint32_t i = 2; i <= limit; ++i) prime_flags[i] = 1;
    for (uint32_t p = 2; (uint64_t)p * p <= limit; ++p) {
        if (prime_flags[p]) {
            for (uint32_t j = p * p; j <= limit; j += p)
                prime_flags[j] = 0;
        }
    }

    /* collect primes */
    for (uint32_t i = 2; i <= limit; ++i)
        if (prime_flags[i]) ++prime_count;
    prime_list = (uint32_t*)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) return;
    size_t k = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (prime_flags[i]) prime_list[k++] = i;

    /* -------- build difference vectors -------- */
    diff_len = (size_t*)calloc(limit + 1, sizeof(size_t));
    diff_vectors = (uint16_t**)calloc(limit + 1, sizeof(uint16_t*));
    if (!diff_len || !diff_vectors) return;

    /* first pass: count pairs for each difference */
    for (size_t idx = 0; idx < prime_count; ++idx) {
        uint32_t p = prime_list[idx];
        for (uint32_t d = 2; d <= limit - p; d += 2)
            if (prime_flags[p + d]) ++diff_len[d];
    }

    /* allocate vectors */
    for (uint32_t d = 0; d <= limit; ++d)
        if (diff_len[d]) diff_vectors[d] = (uint16_t*)malloc(diff_len[d] * sizeof(uint16_t));
        else              diff_vectors[d] = NULL;

    /* second pass: fill vectors */
    size_t *pos = (size_t*)calloc(limit + 1, sizeof(size_t));
    for (size_t idx = 0; idx < prime_count; ++idx) {
        uint32_t p = prime_list[idx];
        for (uint32_t d = 2; d <= limit - p; d += 2)
            if (prime_flags[p + d]) {
                uint16_t *arr = diff_vectors[d];
                size_t ppos   = pos[d];
                arr[ppos++]   = (uint16_t)p;
                pos[d] = ppos;
            }
    }
    free(pos);

    max_limit = limit;
}

/* ----- check that p3 follows p1 immediately for difference d ----- */
static inline bool check_consecutive(uint32_t d, uint32_t p1, uint32_t p3)
{
    if (d > max_limit) return false;
    size_t len = diff_len[d];
    if (len == 0) return false;
    const uint16_t *arr = diff_vectors[d];
    size_t idx = lower_bound_uint16(arr, len, p1);
    if (idx >= len || arr[idx] != (uint16_t)p1) return false;
    if (idx + 1 >= len) return false;
    return arr[idx + 1] == (uint16_t)p3;
}
