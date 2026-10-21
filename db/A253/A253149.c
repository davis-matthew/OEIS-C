/*
 * A253149  Primes >= 256 that remain primes when the digits are reversed in
 *          base 256.
 * https://oeis.org/A253149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 931, u64 n <= 7110, u128 n <= 7110,
 *            big any n
 * Helpers:   grow_seq, is_prime_big, next_prime, rev_bytes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253149:
 *   primes p ≥ 256 for which reversing the 8‑bit words of p (i.e.
 *   the base‑256 representation) yields another prime.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/next_prime.h"   /* idx_t next_prime(uint64_t start); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */
#include "../utils/grow_seq.h"    /* void grow_seq(seq_t **arr, size_t *cap, size_t need); */
#include "../utils/rev_bytes.h"

/* Return the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n==0 */

    /* Static state that persists across calls */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_count = 0;
    static idx_t last_checked = 256; /* next candidate will start after this */

    /* Ensure the cache can hold at least n terms */
    if (cache_count < n) {
        /* Grow the cache appropriately (grow_seq ensures capacity >= need) */
        grow_seq(&cache, &cache_cap, n);

        /* Find more primes until we have n terms */
        while (cache_count < n) {
            /* Get the next prime ≥ last_checked */
            idx_t p64 = next_prime(last_checked);
            seq_t p = (seq_t)p64;

            /* Test whether the reversed 8‑bit word version is also prime */
            if (is_prime_big(rev_bytes(p))) {
                /* Store the prime that satisfies the condition */
                cache[cache_count++] = p;
            }

            /* Set up for the next candidate: start just after the prime we
               have examined. If p64==~(idx_t)0 the next candidate would
               overflow; in that unlikely case we stop. */
            if (p64 == (idx_t)-1) break;          /* wrap‑around, safety check */
            last_checked = p64 + 1;
        }
    }

    return cache[n - 1];
}
