/*
 * A353861  Number of distinct weak run-sums of the prime indices of n.
 * https://oeis.org/A353861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9997, u16 n <= 9997, u32 n <= 9997, u64 n <= 9997,
 *            u128 n <= 9997, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A353861: number of distinct weak run-sums of the prime indices of n.
   For each prime factor p^e of n its prime index k = index of p in the
   sequence of primes is used; all sums k*1, k*2, …, k*e together with 0 are
   collected, sorted and deduplicated, and the number of distinct values is
   returned.  The implementation uses caching of the prime list up to the
   maximum n seen. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
/* Utility headers provided by the harness */
#include "../utils/factor_u64.h"         /* factor_u64 */
#include "../utils/build_primes_upto.h"  /* build_primes_upto */

static uint64_t *prime_list = NULL;      /* cached prime numbers (1‑based) */
static size_t   prime_cnt   = 0;         /* number of primes cached */
static idx_t    prime_max   = 0;         /* maximum prime value cached */

/* Ensure that prime_list contains all primes <= max_limit. */
static void ensure_prime_cache(idx_t max_limit)
{
    if (prime_max >= max_limit)
        return;
    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes_upto(max_limit, &new_primes, &new_cnt);
    free(prime_list);
    prime_list = new_primes;
    prime_cnt  = new_cnt;
    prime_max  = max_limit;
}

/* Return the 1‑based index of prime p in prime_list (assumed to be prime). */
static idx_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p)
            return (idx_t)(mid + 1);
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* p must be present; return 0 on error (should not happen). */
    return 0;
}

/* Comparator for qsort on uint64_t. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t aa = *(const uint64_t *)a;
    uint64_t bb = *(const uint64_t *)b;
    return (aa > bb) - (bb > aa);
}

seq_t a(idx_t n)
{
    if (n < 1U)
        return 0;          /* sequence undefined for n < 1 */

    /* Factorise n.  The array sizes are large enough for n <= 2^64. */
    uint64_t pf[128], pf_exp[128];
    size_t   pf_cnt = 0;
    factor_u64((uint64_t)n, pf, pf_exp, &pf_cnt);

    /* Cache primes needed to map each prime factor to its index. */
    ensure_prime_cache(n);

    /* Collect all weak run‑sum values.  The number of such values is at most
       the sum of the exponents + 1 (including the empty sum 0).  For n <=
       10000 this fits comfortably in a small fixed buffer. */
    uint64_t sums[256];
    size_t sums_len = 0;
    sums[sums_len++] = 0ULL;          /* the empty run */

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p   = pf[i];
        int      exp = (int)pf_exp[i];
        idx_t    idx = prime_index(p);   /* 1‑based index of p */
        for (int k = 1; k <= exp; ++k)
            sums[sums_len++] = (uint64_t)idx * (uint64_t)k;
    }

    /* Sort and deduplicate the sums. */
    qsort(sums, sums_len, sizeof(uint64_t), cmp_uint64);

    seq_t unique = 1;                 /* 0 is always present */
    for (size_t i = 1; i < sums_len; ++i)
        if (sums[i] != sums[i - 1])
            ++unique;

    return unique;
}
