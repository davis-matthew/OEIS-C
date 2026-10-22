/*
 * A334878  For any n > 0 with prime factorization Product_{k > 0}
 *          prime(k)^e_k (where prime(k) denotes the k-th prime number), let
 *          b_k = 1 + max_{k > 0} e_k; a(n) = Sum_{k > 0} e_k * b_k^(k-1).
 * https://oeis.org/A334878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = \sum_k e_k * (1 + max_j e_j)^{k-1}, where n = \prod_k
 *            prime(k)^{e_k} (standard prime k-indexing)
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Helpers:   big_pow, build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

/* static cache of primes up to a fixed limit */
static uint64_t *prime_list   = NULL;
static size_t    prime_count  = 0;
static bool      primes_built = false;
#define PRIME_LIMIT 20000UL     /* sufficient for n up to ~10⁴ (prime≈20000) */

static inline void ensure_primes(void)
{
    if (!primes_built) {
        build_primes_upto(PRIME_LIMIT, &prime_list, &prime_count);
        primes_built = true;
    }
}

static inline idx_t prime_index(uint64_t p)   /* 1‑based index of prime p */
{
    idx_t lo = 0;
    idx_t hi = (idx_t)prime_count;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (prime_list[mid] < p)
            lo = mid + 1;
        else if (prime_list[mid] > p)
            hi = mid;
        else
            return (idx_t)(mid + 1);          /* 1‑based */
    }
    return 0;   /* should never happen for valid prime factors */
}

seq_t a(idx_t n)
{
    if (n <= 1)                /* a(1) = 0 by definition */
        return 0;

    /* factor n into primes and exponents (small arrays are enough for n < 10⁵) */
    uint64_t pf_pr[20];
    uint64_t pf_ex[20];
    size_t   pf_cnt;
    factor_u64((uint64_t)n, pf_pr, pf_ex, &pf_cnt);

    /* compute maximum exponent */
    uint64_t maxe = 0;
    for (size_t i = 0; i < pf_cnt; ++i)
        if (pf_ex[i] > maxe)
            maxe = pf_ex[i];

    /* base b = 1 + maxe, in seq_t */
    seq_t b = (seq_t)(1 + maxe);

    /* ensure prime list available for indexing */
    ensure_primes();

    seq_t result = 0;
    for (size_t i = 0; i < pf_cnt; ++i) {
        seq_t e   = (seq_t)pf_ex[i];
        uint64_t p = pf_pr[i];
        idx_t k    = prime_index(p);          /* 1‑based */
        seq_t pow_val = big_pow(b, (seq_t)(k - 1));
        result += e * pow_val;
    }
    return result;
}
