/*
 * A364053  Zumkeller numbers whose divisors can be partitioned into two
 *          disjoint sets with equal products.
 * https://oeis.org/A364053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer that is a Zumkeller number and whose
 *            number of divisors is a multiple of 4. For each such integer n
 *            we check: 1. σ(n) (sum of all divisors) is even. 2. The divisors
 *            can be partitioned into two disjoint subsets with equal sum
 *            (checked by a bit‑set subset–sum DP). 3. d(n) (...
 * Range:     u8 n <= 3, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, has_equal_subset
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utilities provided by the test harness.  The actual prototypes are
   in the corresponding headers.  Only the needed ones are included. */
#include "../utils/build_primes_upto.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/has_equal_subset.h"

static uint64_t *prime_list = NULL;      /* list of primes for trial factorisation */
static size_t    prime_cnt   = 0;         /* number of primes */
static size_t    prime_limit = 0;         /* maximum prime generated */

/* Ensure that prime_list contains all primes up to at least `limit`. */
static void
ensure_primes(uint64_t limit)
{
    if ((size_t)limit <= prime_limit)
        return;
    free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = (size_t)limit;
}

/* Factorises n using the current prime list.  The caller supplies
   a buffer of size at least 64 for prime factors and exponents. */
static void
factor(uint64_t n, uint64_t *pr, int *ex, size_t *cnt)
{
    size_t c = 0;
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= m; ++i)
    {
        if (m % prime_list[i] == 0)
        {
            int e = 0;
            while (m % prime_list[i] == 0)
            {
                m /= prime_list[i];
                ++e;
            }
            pr[c]   = prime_list[i];
            ex[c]   = e;
            ++c;
        }
    }
    if (m > 1)              /* remaining prime factor */
    {
        if (c == 64)         /* safety guard; should never happen for our range */
            return;
        pr[c]   = m;
        ex[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursively generate all divisors from prime factorisation.  The
   caller must provide a buffer capable of holding all divisors
   (d(n) ≤ 512 for n ≤ 10⁶). */
static void
gen_divs_rec(const uint64_t *pr, const int *ex, size_t pc,
             size_t idx, uint64_t cur, uint64_t *buf, size_t *len)
{
    if (idx == pc)
    {
        buf[(*len)++] = cur;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = 1;
    for (int e = 0; e <= ex[idx]; ++e)
    {
        gen_divs_rec(pr, ex, pc, idx + 1, cur * val, buf, len);
        val *= p;
    }
}

/* Local cache of found sequence terms. */
static seq_t *cache    = NULL;
static size_t  cap     = 0;
static size_t  len     = 0;

/* Append a new term to the cache. */
static void
add_term(uint64_t val)
{
    if (len == cap)
    {
        size_t new_cap = cap ? cap * 2 : 16;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }
    cache[len++] = (seq_t)val;
}

/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)        /* offset is 1, values for n < 1 are undefined */
        return (seq_t)0;

    /* The smallest term in the sequence is 6.  We start searching from
       the next integer after the last found term. */
    static uint64_t curr = 6;

    while (len < (size_t)n)
    {
        /* Prepare prime list for factorising `curr`. */
        uint64_t limit = _floor_isqrt_u64(curr) + 1;
        ensure_primes(limit);

        /* Factorise curr. */
        uint64_t fpr[64];
        int     fex[64];
        size_t  fcnt = 0;
        factor(curr, fpr, fex, &fcnt);

        /* Compute divisor count d(n). */
        uint64_t tau = 1;
        for (size_t i = 0; i < fcnt; ++i)
            tau *= (uint64_t)(fex[i] + 1);

        /* Must be a multiple of 4 (ensures product partition). */
        if ((tau & 3) == 0)
        {
            /* Compute sum of divisors σ(n). */
            seq_t sigma = 1;
            for (size_t i = 0; i < fcnt; ++i)
            {
                seq_t p = fpr[i];
                seq_t prod = 1;
                for (int e = 0; e < fex[i] + 1; ++e)
                    prod *= p;
                sigma *= (prod - 1) / (p - 1);
            }

            /* σ(n) must be even for a sum partition. */
            if ((sigma & 1) == 0)
            {
                seq_t target = sigma / 2;

                /* Generate all divisors. */
                uint64_t divs[512];
                size_t dlen = 0;
                gen_divs_rec(fpr, fex, fcnt, 0, 1, divs, &dlen);

                /* Check for equal‑sum partition. */
                if (has_equal_subset(divs, dlen, target))
                {
                    add_term(curr);
                }
            }
        }

        ++curr;
    }

    return cache[(size_t)n - 1];
}
