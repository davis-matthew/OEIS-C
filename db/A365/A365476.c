/*
 * A365476  a(n) is the minimum of A000120(k)*A000120(A071904(n)/k) for
 *          divisors k of the n-th odd composite number A071904(n) other than
 *          1 and A071904(n).
 * https://oeis.org/A365476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A365476:  a(n) = min_{k | m, 1<k<m} popcount(k) * popcount(m/k)
 * where m is the n-th odd composite number (A071904).  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/isqrt_uint32.h"

/* ----------  sieve and list of odd composites ---------- */
static const uint32_t SIEVE_LIMIT = 10000000;   /* large enough for the tests */
static uint8_t  *is_prime   = NULL;             /* 1 = prime, 0 = composite or >n */
static uint32_t  odd_cap    = 0;
static uint32_t  odd_len    = 0;
static uint32_t  *odd_comps = NULL;
static bool       sieve_done = false;

static void build_sieve_and_comps(void)
{
    if (sieve_done) return;

    is_prime = (uint8_t*)malloc(SIEVE_LIMIT + 1);
    if (!is_prime) exit(1);
    memset(is_prime, 1, SIEVE_LIMIT + 1);
    is_prime[0] = is_prime[1] = 0;

    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;

    odd_cap = 1024;
    odd_len = 0;
    odd_comps = (uint32_t*)malloc(odd_cap * sizeof(uint32_t));
    if (!odd_comps) exit(1);

    for (uint32_t cand = 9; cand <= SIEVE_LIMIT; cand += 2) {
        if (!is_prime[cand]) {                /* odd composite */
            if (odd_len == odd_cap) {
                odd_cap <<= 1;
                odd_comps = (uint32_t*)realloc(odd_comps, odd_cap * sizeof(uint32_t));
                if (!odd_comps) exit(1);
            }
            odd_comps[odd_len++] = cand;
        }
    }
    sieve_done = true;
}

static void ensure_odd_composites(idx_t n)
{
    if (sieve_done) {
        if (odd_len < n) {
            /* for safety, but the chosen limit should be enough for common tests */
            /* If we fall here, try rebuilding with a larger limit (not implemented) */
            exit(1);
        }
    } else {
        build_sieve_and_comps();
        if (odd_len < n) exit(1);   /* should not happen */
    }
}

/* ----------  main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* out of range, defined as 0 */

    ensure_odd_composites(n);

    uint32_t m = odd_comps[n - 1];
    uint32_t sqrt_m = isqrt_uint32(m);

    seq_t min_prod = 0;
    bool first = true;

    for (uint32_t d = 2; d <= sqrt_m; ++d) {
        if (m % d == 0) {
            uint32_t d2 = m / d;
            unsigned int pc1 = __builtin_popcount(d);
            unsigned int pc2 = __builtin_popcount(d2);
            seq_t prod = (seq_t)pc1 * (seq_t)pc2;
            if (first || prod < min_prod) {
                min_prod = prod;
                first = false;
            }
        }
    }
    return min_prod;
}
