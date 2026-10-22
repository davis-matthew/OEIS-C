/*
 * A325120  Sum of binary lengths of the prime indices of n.
 * https://oeis.org/A325120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Σ_{p^e‖n} e * (floor_log2(index(p)) + 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* index(p) is the position of prime p in the increasing sequence of primes   */
/* Note: a(1) = 0                                                   */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/floor_log2_uint64.h"

static uint64_t *PRIMES = NULL;     /* array of primes, 1‑based indices used below    */
static size_t   PRIME_CNT = 0;      /* number of primes in PRIMES                    */
static uint64_t PRIME_MAX = 0;      /* highest prime for which PRIMES is built       */

/* Ensure that PRIMES contains all primes ≤ limit.  */
static void ensure_primes(uint64_t limit)
{
    if (limit <= PRIME_MAX) return;
    if (PRIMES) free(PRIMES);
    build_primes_upto(limit, &PRIMES, &PRIME_CNT);
    PRIME_MAX = limit;
}

/* Return the 1‑based index of prime p.  p is assumed prime.                */
static idx_t prime_index(uint64_t p)
{
    if (p <= PRIME_MAX) {
        size_t lo = 0, hi = PRIME_CNT;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (PRIMES[mid] < p) lo = mid + 1;
            else hi = mid;
        }
        /* Assuming p is prime, it must be found */
        return (idx_t)(lo + 1);
    }
    /* p > current max: extend prime list */
    ensure_primes(p);
    size_t lo = 0, hi = PRIME_CNT;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (PRIMES[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return (idx_t)(lo + 1);
}

/* Factorization: store primes and exponents into pre‑allocated arrays.      */
static void factor_u64(uint64_t n,
                       uint64_t pr[64], uint64_t ex[64], size_t *cnt)
{
    uint64_t remaining = n;
    *cnt = 0;
    if (remaining <= 1) return;
    for (uint64_t q = 2; q*q <= remaining; ++q) {
        if (remaining % q == 0) {
            uint64_t e = 0;
            while (remaining % q == 0) {
                remaining /= q;
                ++e;
            }
            pr[(*cnt)++] = q;
            ex[(*cnt-1)] = e;
        }
    }
    if (remaining > 1) {           /* remaining is prime */
        pr[(*cnt)++] = remaining;
        ex[(*cnt-1)] = 1;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0, a(0) (unused) = 0 */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        idx_t idx = prime_index(p);
        seq_t len = (seq_t)(floor_log2_uint64(idx) + 1); /* binary length of idx */
        sum += (seq_t)e * len;
    }
    return sum;
}
