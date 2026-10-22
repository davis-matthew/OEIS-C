/*
 * A301913  Primes which divide numbers of the form 3^k + 2 for k >= 1.
 * https://oeis.org/A301913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 3765, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow, prime_trial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime that divides some 3^k+2 (OEIS A301913).
   The result is returned in type seq_t, as required by the build
   configuration.  The implementation generates primes incrementally
   and stores the sequence in a static cache so that calls with
   arbitrary ordering of n are handled efficiently.  The function
   is safe for any seq_t that supports the basic arithmetic, boolean
   operations and the mod_pow helper from the utils library. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/prime_trial.h"

/* Dynamically grow/extend a buffer on demand. */
static void ensure_capacity(void **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

/* Compute the multiplicative order of base 3 modulo prime p
   (p is odd and p != 3).  The prime factors of p-1 are supplied
   via the prime list up to the current prime. */
static uint64_t order_3_mod_prime(uint64_t p, uint64_t *primes, size_t prime_cnt)
{
    uint64_t order = p - 1;            /* initial candidate order */
    uint64_t rem = order;              /* remaining part to factor */

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= rem; ++i) {
        uint64_t q = primes[i];
        if (rem % q != 0) continue;

        /* Reduce order by q as long as 3^(order/q) ≡ 1 (mod p). */
        while (order % q == 0) {
            uint64_t cand = order / q;
            if (mod_pow((seq_t)3, cand, (seq_t)p) == 1)
                order = cand;
            else
                break;
        }
        /* Strip q from rem completely for the next outer iteration. */
        while (rem % q == 0) rem /= q;
    }

    /* If a prime factor > sqrt(rem) remains, it divides rem (which < p). */
    if (rem > 1) {
        uint64_t q = rem;
        while (order % q == 0) {
            uint64_t cand = order / q;
            if (mod_pow((seq_t)3, cand, (seq_t)p) == 1)
                order = cand;
            else
                break;
        }
    }

    return order;
}

/* Test whether -2 is a power of 3 modulo prime p. */
static bool has_property(uint64_t p, uint64_t *primes, size_t prime_cnt)
{
    if (p == 3) return false;        /* 3 never divides 3^k+2 */
    uint64_t ord3 = order_3_mod_prime(p, primes, prime_cnt);
    /* If ord(-2) divides ord(3), then (-2)^ord3 ≡ 1 (mod p). */
    return mod_pow((seq_t)(p - 2), ord3, (seq_t)p) == 1;
}

/* Main sequence function as required by the problem statement. */
seq_t a(idx_t n)
{
    /* Static state for incremental generation. */
    static seq_t *seq      = NULL;
    static size_t seq_cap   = 0, seq_len = 0;
    static uint64_t *prime_list = NULL;
    static size_t prime_cap = 0, prime_len = 0;
    static uint64_t next_prime = 5;          /* first odd prime > 3 after init */
    static bool initialized   = false;

    if (!initialized) {
        /* Prime list starts with 2 and 3. */
        ensure_capacity((void**)&prime_list, &prime_cap, 2, sizeof(uint64_t));
        prime_list[0] = 2;
        prime_list[1] = 3;
        prime_len = 2;
        initialized = true;
    }

    /* Grow the sequence cache to at least n elements. */
    ensure_capacity((void**)&seq, &seq_cap, (size_t)n, sizeof(seq_t));

    /* Generate more terms until we have n of them. */
    while (seq_len < (size_t)n) {
        /* Test primality of the current candidate. */
        if (prime_trial(next_prime, prime_list, prime_len)) {
            /* Add the new prime to the list. */
            ensure_capacity((void**)&prime_list, &prime_cap, prime_len + 1, sizeof(uint64_t));
            prime_list[prime_len++] = next_prime;

            /* If it satisfies the property, store it in the sequence. */
            if (has_property(next_prime, prime_list, prime_len))
                seq[seq_len++] = (seq_t)next_prime;
        }
        /* Next odd candidate. */
        next_prime += 2;
    }

    /* Return the requested element (indexing starts at 1). */
    return seq[n - 1];
}
