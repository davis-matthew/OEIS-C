/*
 * A379722  Numbers whose prime indices do not have the same sum as product.
 * https://oeis.org/A379722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Enumerates natural numbers and includes those
 * whose sum of prime indices (counted with multiplicity) is not equal
 * to their product of prime indices.  The seq_t returned is the n‑th
 * such number (a(1)=1).  The routine grows a list of primes on demand
 * to factor candidates efficiently.  It works for any seq_t/idx_t
 * combination that satisfies the O(1) arithmetic and comparison
 * constraints. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;   /* list of primes in ascending order */
static size_t   primes_cnt = 0;   /* number of primes stored           */
static uint64_t primes_limit = 0; /* largest prime currently stored     */

/* Ensure that primes[] contains all primes ≤ limit.  The build function
 * replaces the old array entirely; we free it before reassignment. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= primes_limit)
        return;
    uint64_t *new_primes;
    size_t    new_cnt;
    build_primes_upto(limit, &new_primes, &new_cnt);
    if (primes) {
        free(primes);
    }
    primes = new_primes;
    primes_cnt = new_cnt;
    primes_limit = limit;
}

/* Binary search: return 1‑based index of prime p in primes[],
 * or -1 if p is not found (should never happen). */
static int prime_index(uint64_t p)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        uint64_t q = primes[mid];
        if (q < p) {
            lo = mid + 1;
        } else if (q > p) {
            hi = mid;
        } else {
            return (int)(mid + 1);     /* 1‑based index */
        }
    }
    return -1;   /* not found; only possible if primes array is incomplete */
}

/* Return true if the sum of prime indices (with multiplicity) of n
 * is NOT equal to their product.  For n == 1 the multiset is empty,
 * sum=0, product=1, so the inequality holds. */
static bool qualifies(uint64_t n)
{
    if (n == 1)
        return true;     /* empty multiset: sum 0, product 1 → unequal */

    uint64_t temp = n;
    idx_t sum = 0;
    uint64_t prod = 1;   /* product stays ≤ sum, so fits in 64‑bit */

    /* Ensure we have all primes needed for factorisation up to n. */
    ensure_primes_up_to(n);

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            int idx = prime_index(p);      /* 1‑based index */
            if (idx == -1)
                return false;              /* safety: unrecognised prime */

            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* Update sum and product (do multiplicity e). */
            for (idx_t j = 0; j < e; ++j) {
                sum += (idx_t)idx;
                prod *= (uint64_t)idx;
                /* product exceeds sum ⇒ inequality holds. */
                if (prod > (uint64_t)sum)
                    return true;
            }
        }
    }

    if (temp > 1) {   /* remaining large prime factor */
        int idx = prime_index(temp);
        if (idx == -1)
            return false;
        sum += (idx_t)idx;
        prod *= (uint64_t)idx;
    }

    return prod != (uint64_t)sum;
}

/* Public function: returns the n‑th term of A379722.
 * n is 1‑based. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t candidate = 1;

    while (true) {
        if (qualifies(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
