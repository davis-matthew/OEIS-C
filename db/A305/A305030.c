/*
 * A305030  Number of pairs {k, m} with 0 <= k <= m such that binomial(2k,k) +
 *          binomial(2m,m) is not only a primitive root modulo prime(n) but
 *          also smaller than prime(n).
 * https://oeis.org/A305030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs {k,m} (0 ≤ k ≤ m) such that *
 *            binomial(2k,k) + binomial(2m,m) is a primitive root modulo the *
 *            n‑th prime and the sum is < that prime. The function does a *
 *            direct search over all central binomial coefficients < prime
 * Range:     u8 n <= 54, u16 n <= 5619, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 106 terms through n = 9532 (2026-11-01)
 * Helpers:   binomial, factorize, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/factorize.h"
#include "../utils/powmod.h"

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

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

static bool is_primitive_root(seq_t g, seq_t p, const uint64_t *factors, int cnt)
{
    uint64_t p_val = (uint64_t)p;          /* p is <= 2^32 from get_prime */
    for (int i = 0; i < cnt; ++i) {
        uint64_t q   = factors[i];
        uint64_t exp = (p_val - 1) / q;
        if (powmod(g, exp, p) == (seq_t)1)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0)                       /* offset is 1 – a(1) corresponds to prime(1)=2 */
        return (seq_t)0;

    /* Get the n‑th prime.  It fits in 32 bits. */
    uint32_t p32 = get_prime(n);
    seq_t     p  = (seq_t)p32;

    /* Build an array of all central binomial coefficients < p. */
    seq_t cb[64];
    size_t cb_len = 0;
    for (idx_t k = 0; ; ++k) {
        seq_t val = binomial(2 * k, k);
        if (cmp_seq_t(&val, &p) >= 0)     /* val >= p, stop */
            break;
        if (cb_len < 64)
            cb[cb_len++] = val;
        else
            break;                      /* safety, should never hit 64 for 32‑bit primes */
    }

    /* Factorise p‑1.  Only the distinct prime factors are needed. */
    uint64_t prime_factors[32];
    int      exps[32];
    int      factor_cnt = factorize((uint64_t)p32 - 1, prime_factors, exps);
    if (factor_cnt > 32)          /* safety, should not happen for 32‑bit primes */
        factor_cnt = 32;

    /* Count qualifying pairs. */
    uint64_t count = 0;
    for (size_t i = 0; i < cb_len; ++i) {
        uint64_t bk = (uint64_t)cb[i];   /* cb[i] < p32 ≤ 2^32, fits in 64 bits */
        for (size_t j = i; j < cb_len; ++j) {
            uint64_t sum64 = bk + (uint64_t)cb[j];
            if (sum64 >= p32)            /* sums increase with j */
                break;
            seq_t sum_val = (seq_t)sum64;
            if (is_primitive_root(sum_val, p, prime_factors, factor_cnt))
                ++count;
        }
    }

    return (seq_t)count;
}
