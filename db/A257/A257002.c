/*
 * A257002  Primes p such that p+2 divides p^p+2.
 * https://oeis.org/A257002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 721, u32 n <= 5700, u64 n <= 5700,
 *            u128 n <= 5700, big any n
 * Helpers:   grow_seq_array, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A257002:
   a(n) = the n-th prime p such that (p+2) divides (p^p + 2),
   equivalently 2^p ≡ 2 (mod p+2).  Off‑by‑one: a(1)=7.  */

/* External linkage function required by the evaluation harness. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/mod_pow.h"
#include "../utils/next_prime.h"
#include "../utils/grow_seq_array.h"

/* Cached list of discovered “good” primes. */
static seq_t       *good_primes = NULL; /* array of seq_t holding the primes */
static size_t        good_cnt   = 0;    /* number of primes stored */
static size_t        good_cap   = 0;    /* current allocated capacity */

/* Last prime that was examined; next_prime() will return the prime after this. */
static uint64_t      last_checked = 1;

/* Return the n‑th element of A257002 (1‑based). */
seq_t a(idx_t n)
{
    /* Compute the zero‑based index to fetch.  `n` is guaranteed >= 1. */
    const size_t target = (size_t)(n - 1);

    /* Quick answer if we already have the required prime cached. */
    if (good_cnt > target)
        return good_primes[target];

    /* Continue scanning primes until we have at least `target+1` good primes. */
    while (good_cnt <= target) {
        /* Get the next prime after the last one we inspected. */
        last_checked = next_prime(last_checked);
        uint64_t p = last_checked;

        /* Skip the trivial prime 2, which never satisfies the condition. */
        if (p < 3)
            continue;

        /* Check the congruence 2^p ≡ 2 (mod p+2). */
        seq_t mod  = (seq_t)(p + 2);
        seq_t val  = mod_pow((seq_t)2, p, mod);
        if (val == (seq_t)2) {
            /* Ensure we have room to store the new good prime. */
            if (good_cnt == good_cap) {
                size_t new_cap = (good_cap == 0) ? 16 : good_cap * 2;
                grow_seq_array(&good_primes, &good_cap, new_cap);
            }
            /* Store it. */
            good_primes[good_cnt++] = (seq_t)p;
        }
    }

    /* Return the requested term. */
    return good_primes[target];
}
