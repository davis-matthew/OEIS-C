/*
 * A319521  Completely multiplicative with a(prime(2*k-1)) = prime(k) and
 *          a(prime(2*k)) = 1 for any k > 0 (where prime(k) denotes the k-th
 *          prime number).
 * https://oeis.org/A319521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is completely multiplicative, defined by
 *
 *   a(p) = p                     if p is the (2*k‑1)-st prime,
 *   a(p) = 1                     if p is the (2*k)-st prime,
 *   a(n*m) = a(n)*a(m)           for all n,m.
 *
 *  The function computes a(n) for arbitrary n ≥ 1.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_pi.h"
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/_floor_isqrt_u64.h"

/* Prime list cache ----------------------------------------------------- */
static uint64_t *prime_list = NULL;  /* array of primes */
static size_t prime_cnt   = 0;       /* number of primes stored */
static idx_t  prime_max   = 0;       /* largest prime stored */

/* Ensure that the prime list contains all primes ≤ limit.  */
static void
ensure_primes_upto(idx_t limit)
{
    if (prime_max >= limit)
        return;                       /* already sufficient */

    free(prime_list);                 /* discard old list */
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_max = limit;
}

/* Compute a(p) for a prime p.  The caller guarantees that p is prime. */
static seq_t
a_of_prime(uint64_t p)
{
    /* π(p) is the number of primes ≤ p.  It is returned as a seq_t.  */
    seq_t pi = compute_pi(p);         /* seq_t value */

    /*  If π(p) is even → a(p) = 1 ; else a(p) = prime((π(p)+1)/2). */
    if ((pi & (seq_t)1) == 0)         /* even index */
        return (seq_t)1;

    /* odd index */
    seq_t half = (pi + (seq_t)1) >> 1;      /* (π(p)+1)/2 as seq_t */
    idx_t k = (idx_t)half;                  /* fits in idx_t */
    return get_prime(k);
}

/* Fast exponentiation of seq_t base raised to uint64_t exp.  */
static seq_t
seq_pow_seq(seq_t base, uint64_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1ULL)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

/* Main sequence function ------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;           /* offset is 1 */

    uint64_t rem = n;
    seq_t prod = (seq_t)1;

    /* Factor rem using the cached prime list up to sqrt(rem). */
    idx_t limit = _floor_isqrt_u64(rem);
    if (limit < 2)
        limit = 2;
    ensure_primes_upto(limit);

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= rem; ++i) {
        uint64_t p = prime_list[i];
        if (rem % p != 0)
            continue;

        /* Count multiplicity of p. */
        uint64_t exp = 0;
        while (rem % p == 0) {
            rem /= p;
            ++exp;
        }

        seq_t ap = a_of_prime(p);
        prod *= seq_pow_seq(ap, exp);
    }

    /* If rem > 1 now, it is a prime > sqrt(original n). */
    if (rem > 1) {
        seq_t ap = a_of_prime(rem);
        prod *= ap;
    }

    return prod;
}
