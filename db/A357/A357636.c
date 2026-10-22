/*
 * A357636  Numbers k such that the skew-alternating sum of the partition
 *          having Heinz number k is 0.
 * https://oeis.org/A357636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A357636: Numbers k such that the skew‑alternating sum of the partition
   having Heinz number k is 0.  The terms are obtained by brute‑force searching
   k ≥ 1 and testing the skew‑alternating sum pattern. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------ */
/* Prime list handling                                               */
/* ------------------------------------------------------------------ */

static uint64_t *prime_list = NULL;    /* array of primes in increasing order */
static size_t     prime_cnt  = 0;      /* number of primes in the list      */
static idx_t      prime_limit = 0;     /* largest prime in prime_list       */

/* Ensure that prime_list contains all primes ≤ limit.  The list is
   rebuilt if necessary; previously allocated memory is freed. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit && prime_list != NULL)
        return;

    size_t cnt;
    uint64_t *tmp = NULL;
    build_primes_upto(limit, &tmp, &cnt);
    free(prime_list);
    prime_list = tmp;
    prime_cnt  = cnt;
    prime_limit = limit;
}

/* Return the (1‑based) index of prime p in prime_list.  p is assumed
   to be a prime ≤ prime_limit.  If p is not found, returns -1. */
static int prime_index_of(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p)
            return (int)(mid + 1);    /* indices are 1‑based */
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return -1;
}

/* ------------------------------------------------------------------ */
/* Skew‑alternating sum test                                         */
/* ------------------------------------------------------------------ */

static bool qualifies(idx_t k)
{
    if (k == 1)         /* empty partition, sum = 0 */
        return true;

    /* factors are collected as indices (1‑based) of their primes.  */
    int  indices[64];
    int  idx_cnt = 0;

    idx_t tmp = k;

    /* trial division by primes up to sqrt(tmp) */
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p != 0)
            continue;

        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        int idx = prime_index_of(p);
        for (int e = 0; e < exp; ++e)
            indices[idx_cnt++] = idx;
    }

    /* remaining factor > 1 (prime) */
    if (tmp > 1) {
        if (tmp > (uint64_t)prime_limit)
            /* extend prime list to include this new prime */
            ensure_primes_upto(tmp);

        int idx = prime_index_of(tmp);
        indices[idx_cnt++] = idx;
    }

    /* sort indices descending (small fixed size, insertion sort) */
    for (int i = 1; i < idx_cnt; ++i) {
        int key = indices[i];
        int j = i - 1;
        while (j >= 0 && indices[j] < key) {
            indices[j + 1] = indices[j];
            --j;
        }
        indices[j + 1] = key;
    }

    /* compute skew‑alternating sum */
    int64_t sum = 0;
    for (int pos = 0; pos < idx_cnt; ++pos) {
        int sign = ((pos & 3) == 0 || (pos & 3) == 3) ? 1 : -1;
        sum += sign * (int64_t)indices[pos];
    }
    return sum == 0;
}

/* ------------------------------------------------------------------ */
/* Cache of generated sequence                                       */
/* ------------------------------------------------------------------ */

static seq_t *cache = NULL;
static size_t cache_cnt = 0;    /* number of values in cache   */
static idx_t  next_k   = 1;     /* next integer to test       */

/* One‑time initialization of prime list */
static void initialize(void)
{
    static bool init_done = false;
    if (init_done)
        return;
    /* Build an initial list of primes up to 1,000,000;
       this should be sufficient for n ≤ 10 000. */
    ensure_primes_upto(1000000);
    init_done = true;
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                           */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1; return 0 */
        return (seq_t)0;

    initialize();

    /* return cached value if already computed */
    if (n <= cache_cnt)
        return cache[n - 1];

    /* generate additional terms until we have n of them */
    while (cache_cnt < n) {
        if (qualifies(next_k)) {
            seq_t *tmp = (seq_t *)realloc(cache,
                                         (cache_cnt + 1) * sizeof(seq_t));
            if (!tmp) {
                free(cache);
                exit(1);   /* allocation failure */
            }
            cache = tmp;
            cache[cache_cnt++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
