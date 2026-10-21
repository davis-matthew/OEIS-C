/*
 * A274771  Number of primes <= n-th Carmichael lambda number.
 * https://oeis.org/A274771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π(λ(n))
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   build_primes_upto, compute_pi, floor_isqrt_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* λ(n) is Carmichael's lambda function; π(m) counts primes ≤ m. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/floor_isqrt_u64.h"
#include "../utils/gcd.h"
#include "../utils/compute_pi.h"

static uint64_t *prime_cache = NULL;   /* primes up to current limit   */
static size_t   prime_cnt   = 0;        /* number of cached primes      */
static uint64_t prime_limit = 0;       /* current upper bound of primes*/

/* Ensure that prime_cache contains all primes ≤ limit.  */
static void ensure_primes(uint64_t limit)
{
    if (limit <= prime_limit)
        return;

    if (prime_cache != NULL)
        free(prime_cache);

    build_primes_upto(limit, &prime_cache, &prime_cnt);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* safety for offsets below 1 */
        return 0;

    /* ----- compute λ(n) ----- */
    uint64_t nn     = (uint64_t)n;
    uint64_t sqrt_n = floor_isqrt_u64(nn);
    ensure_primes(sqrt_n + 1);        /* primes up to √n are enough */

    uint64_t m     = nn;
    seq_t    lam   = 1;               /* current λ value */

    /* factor n using trial division with the cached primes */
    for (size_t i = 0; i < prime_cnt && prime_cache[i] != 0; ++i) {
        uint64_t p = prime_cache[i];
        if (p * p > m)
            break;

        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }

            /* contribution of p^e to λ */
            seq_t contrib;
            if (p == 2) {
                if (e == 1)
                    contrib = 1;                 /* λ(2)   = 1      */
                else if (e == 2)
                    contrib = 2;                 /* λ(4)   = 2      */
                else
                    contrib = (seq_t)1 << (e - 2);/* λ(2^e) = 2^(e-2) */
            } else {
                /* λ(p^e) = φ(p^e) = p^(e-1)(p-1) */
                seq_t pow = 1;
                for (uint64_t k = 0; k < e - 1; ++k)
                    pow *= (seq_t)p;
                contrib = pow * ((seq_t)p - 1);
            }

            seq_t g    = gcd(lam, contrib);
            lam = (lam / g) * contrib;          /* lcm = λ/g * contrib */
        }
    }

    /* If a prime factor > √nn remains, it is unique and occurs with e=1 */
    if (m > 1) {
        uint64_t p = m;
        seq_t contrib = (p == 2) ? 1 : (seq_t)p - 1; /* λ(p) = φ(p) = p-1 (except p=2) */
        seq_t g = gcd(lam, contrib);
        lam = (lam / g) * contrib;
    }

    /* ----- count primes ≤ λ(n) ----- */
    return compute_pi((idx_t)lam);
}
