/*
 * A359210  Number of m^k == 1 (mod p) for 0 < m,k < p where p is the n-th
 *          prime.
 * https://oeis.org/A359210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p-1) * sum_{d|p-1} φ(d)/d, where p is the n-th prime
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation enumerates all divisors of p-1 using its prime factorisation
 * and accumulates the required sum exactly in seq_t. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static void dfs_divisors(size_t idx, seq_t d, seq_t phi,
                         const uint64_t *pr, const uint64_t *ex,
                         size_t cnt, seq_t *sum, seq_t t)
{
    if (idx == cnt) {
        /* add (t / d) * phi to the sum */
        *sum += (t / d) * phi;
        return;
    }
    seq_t cur_d = d;
    seq_t cur_phi = phi;
    /* exponent 0 */
    dfs_divisors(idx + 1, cur_d, cur_phi, pr, ex, cnt, sum, t);
    /* exponents 1 .. ex[idx] */
    for (size_t e = 1; e <= ex[idx]; ++e) {
        cur_d *= (seq_t)pr[idx];
        if (e == 1)
            cur_phi *= (seq_t)(pr[idx] - 1);
        else
            cur_phi *= (seq_t)pr[idx];
        dfs_divisors(idx + 1, cur_d, cur_phi, pr, ex, cnt, sum, t);
    }
}

seq_t a(idx_t n)
{
    /* Obtain the n-th prime (1-based). */
    uint32_t p = get_prime(n);
    /* t = p-1. The values are small for n <= 10000, but we keep them in seq_t. */
    seq_t t = (seq_t)p - 1u;

    /* Factorise t. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    if (t > 0)
        factor_u64((uint64_t)t, primes, exps, &cnt);
    else {
        /* For p = 2, t = 1, which has no prime factors. */
        cnt = 0;
    }

    /* Enumerate all divisors and accumulate the sum. */
    seq_t sum = 0;
    dfs_divisors(0, (seq_t)1, (seq_t)1, primes, exps, cnt, &sum, t);

    return sum;
}
