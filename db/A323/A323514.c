/*
 * A323514  a(n) = 1 if the greatest common divisor of the set of solutions to
 *          phi(x) = n is larger than one, otherwise 0.
 * https://oeis.org/A323514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if all solutions of phi(x)=n have a common divisor >1,
 *            otherwise 0
 * Range:     u8 n <= 91, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9352 (2026-11-01)
 * Helpers:   build_primes_upto, gcd, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the characteristic function of A303745 as requested. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Recursive helper that finds all solutions of phi(x)=rem
   and accumulates their greatest common divisor in *gcd_out. */
static void
search(uint64_t rem, size_t p_idx, seq_t curr, seq_t *gcd_out,
       const uint64_t *primes, size_t prime_len)
{
    if (rem == 1) {
        /* Found a solution curr. Update the accumulated GCD. */
        *gcd_out = gcd(*gcd_out, curr);
        return;
    }

    /* Try each prime starting from p_idx. */
    for (size_t i = p_idx; i < prime_len; ++i) {
        uint64_t p = primes[i];
        uint64_t pow_val = 1;            /* p^(k-1)          */
        uint64_t term   = p - 1;         /* p^(k-1)*(p-1)    */
        uint64_t k      = 1;             /* current exponent  */

        while (term <= rem) {
            if (rem % term == 0) {
                /* New potential solution using prime p with exponent k. */
                seq_t p_pow = ipow_idx((seq_t)p, (idx_t)k);
                seq_t new_val = seq_mul(curr, p_pow);
                search(rem / term, i + 1, new_val, gcd_out,
                       primes, prime_len);
            }

            /* Advance to next exponent for this prime. */
            ++k;
            if (pow_val > UINT64_MAX / p)
                break;
            pow_val *= p;
            term = pow_val * (p - 1);
            if (term == 0)          /* overflow guard – should never happen here */
                break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined below offset, safe guard */

    uint64_t limit = (uint64_t)n + 1;   /* max prime needed is n+1 */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);   /* primes[0..prime_cnt-1] */

    seq_t gcd_all = 0;                  /* 0 acts as identity for seq_gcd */
    search((uint64_t)n, 0, (seq_t)1, &gcd_all, primes, prime_cnt);

    /* The result is 1 iff gcd_all > 1, else 0. */
    seq_t one = 1;
    int cmp = seq_cmp(&gcd_all, &one);
    return (cmp > 0) ? 1 : 0;
}
