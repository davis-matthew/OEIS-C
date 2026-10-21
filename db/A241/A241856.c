/*
 * A241856  Smallest number of carries while adding two primes in binary, the
 *          sum of which is 2*n.
 * https://oeis.org/A241856
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal number of binary carries when adding two primes
 *            whose sum is 2·n
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   count_carries_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/count_carries_uint64.h"

/* ---------- Prime sieve management ---------- */
static idx_t sieve_limit = 0;
static unsigned char *is_comp = NULL;   /* 0 = prime, 1 = composite */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

static void rebuild_sieve(idx_t limit)
{
    /* free previous data */
    free(is_comp);
    free(prime_list);

    sieve_limit = limit;
    size_t sz = (size_t)limit + 1;
    is_comp = (unsigned char *)malloc(sz * sizeof(unsigned char));
    if (!is_comp)
        return;
    for (size_t i = 0; i < sz; ++i)
        is_comp[i] = 0;

    /* allocate enough space for all primes (upper bound: limit+10) */
    prime_list = (uint64_t *)malloc((sz + 10) * sizeof(uint64_t));
    if (!prime_list)
        return;
    prime_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[(size_t)i]) {
            prime_list[prime_cnt++] = (uint64_t)i;
            if ((uint64_t)i * (uint64_t)i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }
}

static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;
    rebuild_sieve(limit);
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;     /* sequence is defined only for n >= 2 */

    idx_t target = 2 * n;
    ensure_sieve(target);     /* generate primes up to 2·n */

    int min_carries = INT_MAX;

    /* Search all prime pairs (p,q) with p + q = 2·n, p ≤ q. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > n)   /* if p > n then q < p and we would have processed it already */
            break;
        uint64_t q = target - p;
        if (q < p)
            continue;
        if (q <= (uint64_t)sieve_limit && !is_comp[(size_t)q]) {
            int c = count_carries_uint64(p, q);
            if (c < min_carries)
                min_carries = c;
            if (min_carries == 0)   /* can't do better than zero */
                break;
        }
    }

    /* The value fits into seq_t because min_carries is small (≤64).
       Casting is safe for all supported seq_t types. */
    return (seq_t)min_carries;
}
