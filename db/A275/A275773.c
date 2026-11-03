/*
 * A275773  Primes p congruent to 1 modulo 13 such that x^13 = 2 has a
 *          solution modulo p.
 * https://oeis.org/A275773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   mem, next_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of primes p ≡ 1 (mod 13) for which the congruence
   x^13 ≡ 2 (mod p) has a solution.  For a prime p ≡ 1 (mod 13),
   2 is a 13‑th power residue modulo p iff
        2^((p‑1)/13) ≡ 1 (mod p).
   The function below builds the list of such primes on demand, storing
   them in a static dynamic array so that calls can be made in any
   order. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/next_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t last_prime = 1;
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
    }

    /* Build entries until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Get the next prime after last_prime. */
        last_prime = next_prime(last_prime);

        /* We only care about primes ≡ 1 (mod 13). */
        if (last_prime % 13 != 1) {
            continue;
        }

        /* Check the 13-th order condition. */
        idx_t exp = (last_prime - 1) / 13;
        if (powmod((seq_t)2, exp, (seq_t)last_prime) == (seq_t)1) {
            /* Store the prime. */
            append_candidate(&cache, &cache_len, &cache_cap,
                             (seq_t)last_prime);
        }
    }

    /* Return the requested term (indexing starts at 1). */
    return cache[n - 1];
}
