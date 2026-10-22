/*
 * A351543  Even numbers k such that there is an odd prime p that divides
 *          sigma(k), but valuation(k, p) differs from valuation(sigma(k), p),
 *          and p does not divide A003961(k), where A003961 is fully
 *          multiplicative with a(p) = nextprime(p), and sigma is the sum of
 *          divisors function.
 * https://oeis.org/A351543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth even integer k satisfying the A351543 condition:
 *            there exists an odd prime p dividing sigma(k) with
 *            valuation(p,k) ≠ valuation(p,sigma(k)), and p does not divide
 *            A003961(k) (i.e., p ≠ nextprime(r) for any prime divisor r of k)
 * Range:     u8 n <= 81, u16 n <= 9435, u32 n <= 9435, u64 n <= 9435,
 *            u128 n <= 9435, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"      /* factor_u64 */
#include "../utils/next_prime.h"      /* next_prime */

/* ---------- helper to test a single even k ---------- */
static bool satisfies_even(uint64_t k)
{
    /* factor k */
    uint64_t pk[64];
    uint64_t ke[64];
    size_t kcnt = 0;
    factor_u64(k, pk, ke, &kcnt);

    /* compute sigma(k) as uint64_t (k ≤ 200000 ⇒ sigma(k) fits in 64 bits) */
    uint64_t sigma = 1;
    for (size_t i = 0; i < kcnt; ++i) {
        uint64_t term = 1;
        uint64_t power = 1;
        for (int e = 1; e <= (int)ke[i]; ++e) {
            power *= pk[i];
            term += power;
        }
        sigma *= term;
    }

    /* factor sigma */
    uint64_t sp[64];
    uint64_t se[64];
    size_t scnt = 0;
    factor_u64(sigma, sp, se, &scnt);

    /* precompute nextprime for each prime divisor of k */
    uint64_t nextp[64];
    for (size_t i = 0; i < kcnt; ++i)
        nextp[i] = next_prime(pk[i]);

    /* examine odd prime factors of sigma */
    for (size_t i = 0; i < scnt; ++i) {
        if ((sp[i] & 1ULL) == 0)   /* skip even primes */
            continue;
        uint64_t p = sp[i];
        uint64_t v_sigma = se[i];
        uint64_t v_k = 0;
        for (size_t j = 0; j < kcnt; ++j)
            if (pk[j] == p) { v_k = ke[j]; break; }
        if (v_sigma == v_k)            /* valuations must differ */
            continue;
        /* check that p does not divide A003961(k) */
        bool divides_A003961 = false;
        for (size_t j = 0; j < kcnt; ++j)
            if (nextp[j] == p) { divides_A003961 = true; break; }
        if (divides_A003961)
            continue;
        /* found a suitable prime */
        return true;
    }
    return false;
}

/* ---------- global state for memoization ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static uint64_t current_k = 4;   /* candidate even k starts at 4 */

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for 0, return 0 */

    /* generate terms until we have n terms */
    while (cache_len < (size_t)n) {
        /* find next even k that satisfies the condition */
        while (!satisfies_even(current_k))
            current_k += 2;

        /* store the found value */
        seq_t val = (seq_t)current_k;
        size_t new_cap = cache_len + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache[cache_len++] = val;

        /* advance to next even candidate */
        current_k += 2;
    }
    return cache[n - 1];   /* a is 1-indexed */
}
