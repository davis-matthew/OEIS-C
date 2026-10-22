/*
 * A329119  Orders of the finite groups SL_2(K) when K is a finite field with
 *          q = A246655(n) elements.
 * https://oeis.org/A329119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = q*(q^2-1) where q = A246655(n) (the n‑th prime power)
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 285, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_t_wrapper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation generates all prime powers up to a
    sufficient limit using a simple sieve, stores them in a
    static array, sorts the array, and then returns the
    required value.  A cache grows on demand so that any order
    of calls is supported.  */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/cmp_seq_t_wrapper.h"

/* static storage for the prime–power sequence */
static seq_t   *prime_powers = NULL;   /* array of prime powers */
static size_t   pp_length    = 0;       /* number of prime powers stored */
static size_t   pp_capacity  = 0;       /* allocated capacity of prime_powers */

/* helper: enlarge the prime‑power cache so that it contains at
   least the first n terms. */
static void ensure_prime_powers(idx_t n)
{
    if (pp_length >= (size_t)n) return;              /* already enough */

    /* first, find a limit that is large enough to contain at least n
       prime powers.  We start with 128 and double until we have
       enough. */
    uint64_t limit = 128;
    size_t   need  = 0;

    while (1) {
        /* sieve to find primes up to limit */
        uint8_t *is_composite = (uint8_t *)calloc(limit + 1, 1);
        if (!is_composite) return;                   /* out of memory */
        is_composite[0] = is_composite[1] = 1;
        for (uint64_t i = 2; i * i <= limit; ++i) {
            if (!is_composite[i]) {
                for (uint64_t j = i * i; j <= limit; j += i) {
                    is_composite[j] = 1;
                }
            }
        }

        /* count prime powers up to the limit */
        need = 0;
        for (uint64_t p = 2; p <= limit; ++p) {
            if (!is_composite[p]) {
                uint64_t val = p;
                while (val <= limit) {
                    ++need;
                    if (val > limit / p) break;
                    val *= p;
                }
            }
        }
        free(is_composite);
        if (need >= (size_t)n) break;   /* enough */
        limit <<= 1; /* double the limit and try again */
    }

    /* regenerate prime powers up to the chosen limit and store them */
    uint8_t *is_composite = (uint8_t *)calloc(limit + 1, 1);
    if (!is_composite) return;
    is_composite[0] = is_composite[1] = 1;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                is_composite[j] = 1;
            }
        }
    }

    /* allocate (or re‑allocate) sufficient space */
    seq_t *new_arr = (seq_t *)realloc(prime_powers, need * sizeof(seq_t));
    if (!new_arr) {
        free(is_composite);
        return; /* allocation failed, give up */
    }
    prime_powers = new_arr;
    pp_capacity  = need;
    pp_length    = 0;   /* we will refill */

    /* collect the prime powers */
    for (uint64_t p = 2; p <= limit; ++p) {
        if (!is_composite[p]) {
            uint64_t val = p;
            while (val <= limit) {
                prime_powers[pp_length++] = (seq_t)val;
                if (val > limit / p) break;
                val *= p;
            }
        }
    }
    free(is_composite);

    /* sort the array so that it is in increasing order */
    qsort(prime_powers, pp_length, sizeof(seq_t), cmp_seq_t_wrapper);
}

/* required function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for non‑positive indices */

    ensure_prime_powers(n);

    seq_t q     = prime_powers[(size_t)n - 1];
    seq_t q_sq  = q * q;
    return q * (q_sq - 1);
}
