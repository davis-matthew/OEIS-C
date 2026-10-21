/*
 * A262286  Primes whose binary expansion begins 100.
 * https://oeis.org/A262286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1679, u32 n <= 9937, u64 n <= 9937,
 *            u128 n <= 9937, big any n
 * Helpers:   append_candidate, bitlen_seq_t, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate integers, test primality, keep those whose
   most‑significant three bits are 1,0,0 (binary starts 100). */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* memoisation state */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  last_checked = 0;  /* last integer examined */

    /* generate further terms until we have at least n */
    while (cache_len < (size_t)n) {
        ++last_checked;

        /* skip numbers too small to have a 3‑bit prefix */
        if (last_checked < 8) continue;

        /* is it a prime? */
        if (!is_prime(last_checked)) continue;

        seq_t p = (seq_t)last_checked;
        unsigned bits = bitlen_seq_t(p);

        /* check the leading 100 pattern */
        if (bits >= 3 && ((p >> (bits - 3)) == (seq_t)4))
            append_candidate(&cache, &cache_len, &cache_cap, p);
    }

    /* cache is zero‑based indexed */
    return cache[(size_t)n - 1];
}
