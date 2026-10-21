/*
 * A273284  A273282(n)^Omega(n), where Omega = A001222.
 * https://oeis.org/A273284
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, bigomega, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS A273284: a(n) = (A273282(n))^Ω(n)
 *  where Ω(n) = total number of prime factors of n (counted with multiplicity)
 *  and A273282(n) = largest prime p such that p^Ω(n) ≤ n.
 *
 *  For each n ≥ 2 this function returns a(n) exactly.
 *  It uses a cache of primes up to the largest root seen so far,
 *  and a binary search to find the correct prime.  All arithmetic
 *  is performed with seq_t as required by the build.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"      /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/bigomega.h"    /* seq_t bigomega(idx_t n); */
#include "../utils/is_prime_small.h" /* bool is_prime_small(seq_t n); */

static uint64_t *prime_list = NULL;   /* primes <= current_limit, sorted */
static size_t prime_count  = 0;
static idx_t  current_limit = 0;

/* Build a prime sieve up to 'limit' (inclusive).  Resulting array is
 * sorted ascending.  Call only when 'limit' > current_limit. */
static void ensure_prime_sieve(idx_t limit)
{
    if (limit <= current_limit)
        return;

    /* free previous list if any */
    free((void*)prime_list);

    /* Simple sieve of Eratosthenes */
    size_t sz = (size_t)limit + 1;
    bool *is_composite = (bool*)malloc(sz);
    for (size_t i = 0; i < sz; ++i)
        is_composite[i] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_composite[(size_t)i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[(size_t)j] = true;

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[(size_t)i])
            ++cnt;

    /* Allocate result array */
    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    prime_count = cnt;
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[(size_t)i])
            prime_list[k++] = (uint64_t)i;

    free(is_composite);
    current_limit = limit;
}

/* Integer k-th root of n: floor(n^(1/k)).  Using binary search and big_pow. */
static seq_t kth_root(seq_t n, idx_t k)
{
    seq_t low  = 1;
    seq_t high = n;
    seq_t ans  = 1;

    while (low <= high) {
        seq_t mid = (low + high) / 2;
        seq_t powmid = big_pow(mid, (seq_t)k);

        if (powmid <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2)          /* behaviour below offset irrelevant */
        return 0;

    seq_t omega_seq = bigomega(n);      /* Ω(n) as seq_t */
    idx_t omega = (idx_t)omega_seq;     /* small enough for idx_t */

    /* Compute floor(n^(1/ω)) */
    seq_t root = kth_root((seq_t)n, omega);
    uint64_t root_uint = (uint64_t)root;    /* for prime search */

    /* Ensure primes up to root */
    ensure_prime_sieve((idx_t)root_uint);

    /* Binary search largest prime ≤ root */
    size_t lo = 0, hi = prime_count - 1;
    size_t best = 0;
    while (lo <= hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_list[mid] <= root_uint) {
            best = mid;
            lo   = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    seq_t p = (seq_t)prime_list[best];

    return big_pow(p, (seq_t)omega);
}
