/*
 * A366128  Least non-subset-sum of the multiset of prime indices of n.
 * https://oeis.org/A366128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer ≤ sum of prime indices of n that
 *            is not a subset sum of the multiset of those prime indices; 0 if
 *            every such integer is attainable
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"

/* Global tables */
static idx_t *prime_tbl = NULL;
static size_t prime_cnt = 0;
static bool primes_ready = false;

/* Build a prime table sufficient for n up to about 10000.
   The 10000th prime is 104729, and primes up to 12000 cover all indices. */
static void
ensure_primes(void)
{
    if (primes_ready)
        return;
    idx_t limit = 12000;
    build_primes(limit, &prime_tbl, &prime_cnt);
    primes_ready = true;
}

/* Return the 0‑based index of val in prime_tbl (assumes val is prime). */
static size_t
lower_bound_prime_idx(uint64_t val)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_tbl[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;   /* guaranteed to find an exact match */
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;

    /* n==1 has an empty multiset → a(1)=0 */
    if (n == 1)
        return 0;

    ensure_primes();

    idx_t tmp = n;
    /* At most as many distinct prime factors as log₂(n). */
    idx_t indices[64];
    idx_t exps[64];
    size_t nf = 0;

    /* factorise n */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = prime_tbl[pi];
        if (p * p > tmp)
            break;
        if (tmp % p == 0) {
            idx_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            indices[nf] = lower_bound_prime_idx((uint64_t)p) + 1;  /* 1‑based index */
            exps[nf]    = cnt;
            ++nf;
        }
    }
    if (tmp > 1) { /* remaining prime > sqrt(original n) */
        indices[nf] = lower_bound_prime_idx((uint64_t)tmp) + 1;
        exps[nf]    = 1;
        ++nf;
    }

    /* Compute total sum of the multiset */
    seq_t total = 0;
    for (size_t i = 0; i < nf; ++i) {
        total += (seq_t)indices[i] * (seq_t)exps[i];
    }
    if (total == 0)
        return 0;

    size_t tot = (size_t)total;        /* fits for n ≤ 10000 */
    bool *dp  = (bool *)calloc(tot + 1, sizeof(bool));
    if (!dp)
        return 0;                     /* unlikely allocation failure */
    dp[0] = true;

    /* Bounded subset‑sum DP */
    for (size_t i = 0; i < nf; ++i) {
        size_t idx_s = (size_t)indices[i];
        idx_t   cnt  = exps[i];
        for (idx_t k = 0; k < cnt; ++k) {
            for (size_t v = tot; v >= idx_s; --v) {
                if (dp[v - idx_s])
                    dp[v] = true;
            }
        }
    }

    seq_t result = 0;
    for (size_t x = 1; x <= tot; ++x) {
        if (!dp[x]) {
            result = (seq_t)x;
            break;
        }
    }

    free(dp);
    return result;
}
