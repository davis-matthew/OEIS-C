/*
 * A344616  Alternating sum of the integer partition with Heinz number n.
 * https://oeis.org/A344616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = alternating sum of the partition with Heinz number n (the
 *            partition obtained by writing n as a product of primes, mapping
 *            prime → its index in the prime list). The sequence is defined
 *            for n ≥ 1, with a(1) = 0
 * Range:     u8 n <= 1618, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utility headers supplied by the build */
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* Global state: list of primes for indexing */
static uint64_t *prime_list = NULL;   /* primes[0] = 2, primes[1] = 3, … */
static size_t    prime_cnt   = 0;     /* number of primes stored */
static uint64_t  prime_limit = 0;     /* largest prime currently stored */

/* Ensure we have all primes ≤ limit in prime_list.  */
static void
ensure_primes_upto(uint64_t limit)
{
    if (prime_limit >= limit) return;

    /* If we already had a list, free it before allocating a new one. */
    if (prime_list != NULL) {
        free(prime_list);
        prime_list = NULL;
        prime_cnt = 0;
    }

    /* build_primes_upto allocates a new array of primes ≤ limit. */
    build_primes_upto(limit, &prime_list, &prime_cnt);

    prime_limit = limit;
}

/* Binary search on the sorted prime_list.  Return 1‑based index. */
static size_t
prime_index_of(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        uint64_t val = prime_list[mid];
        if (val == p) return mid + 1;   /* convert to 1‑based */
        if (val < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0; /* should never happen for a prime */
}

/* Comparator for qsort to sort unsigned 64‑bit integers descending. */
static int
cmp_desc(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    if (x < y) return 1;
    if (x > y) return -1;
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* sequence starts at offset 1 */

    /* Factor n into primes and their exponents. */
    uint64_t factors[64];
    uint64_t exps[64];
    size_t num_factors = 0;
    factor_u64((uint64_t)n, factors, exps, &num_factors);

    /* Prepare an array to hold all partition parts (indices of primes). */
    uint64_t parts[128];      /* maximum number of parts is small */
    size_t part_cnt = 0;

    for (size_t i = 0; i < num_factors; ++i) {
        uint64_t p = factors[i];
        uint64_t e = exps[i];

        /* Ensure we can look up the index of prime p. */
        if (prime_list == NULL || prime_limit < p)
            ensure_primes_upto(p);

        size_t idx = prime_index_of(p);

        /* Append idx e times. */
        for (uint64_t j = 0; j < e; ++j) {
            parts[part_cnt++] = idx;
        }
    }

    if (part_cnt == 0) return (seq_t)0;   /* n == 1 */

    /* Sort parts descending to obtain the standard partition order. */
    qsort(parts, part_cnt, sizeof(uint64_t), cmp_desc);

    /* Compute the alternating sum: sum_{i} (-1)^(i-1) parts[i]. */
    long long sum = 0;
    for (size_t i = 0; i < part_cnt; ++i) {
        if ((i & 1) == 0)
            sum += (long long)parts[i];
        else
            sum -= (long long)parts[i];
    }

    return (seq_t)sum;
}
