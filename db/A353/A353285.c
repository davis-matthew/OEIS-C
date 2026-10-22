/*
 * A353285  Consider the number of divisors tau(k) of every composite k
 *          between prime p >= 3 and the next prime; if the largest tau(k) is
 *          a square, then p is in the sequence.
 * https://oeis.org/A353285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   divisor_count_u64, int_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353285
   A(n) = the n-th prime p (p ≥ 3) for which the maximum τ(k) over
          all composite k in the interval (p , next_prime(p))
          is a perfect square. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/int_sqrt.h"

/* Cached sequence values and state */
static seq_t *seq_cache   = NULL;   /* array of terms */
static size_t seq_len     = 0;      /* number of terms stored */
static size_t seq_cap     = 0;      /* allocated capacity */

static uint64_t last_used_prime = 2;  /* last prime processed */

/* Ensure the array contains at least n terms */
static void
ensure_terms(idx_t n)
{
    while (seq_len < n) {
        /* get next prime after last_used_prime */
        uint64_t p = next_prime(last_used_prime);
        last_used_prime = p;

        /* require p ≥ 3 */
        if (p < 3)
            continue;

        /* the next prime after p */
        uint64_t q = next_prime(p);

        /* find maximum τ(k) for composites in (p , q) */
        seq_t maxTau  = (seq_t)0;
        for (uint64_t k = p + 1; k < q; ++k) {
            seq_t tau = divisor_count_u64(k);
            if (tau > maxTau)
                maxTau = tau;
        }

        /* test whether maxTau is a square */
        if (maxTau > (seq_t)0) {
            idx_t r = int_sqrt((idx_t)maxTau);
            if ((seq_t)r * (seq_t)r == maxTau) {
                /* store the term */
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap ? seq_cap << 1 : 8;
                    seq_cache = (seq_t *)realloc(seq_cache,
                                                 seq_cap * sizeof(seq_t));
                    if (!seq_cache) abort();
                }
                seq_cache[seq_len++] = (seq_t)p;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1 */
        return 0;

    if (!seq_cache) {      /* first call – initialize cache */
        seq_cache = (seq_t *)malloc(8 * sizeof(seq_t));
        if (!seq_cache) abort();
        seq_cap = 8;
    }

    ensure_terms(n);
    return seq_cache[n - 1];
}
