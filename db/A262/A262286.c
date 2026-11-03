/*
 * A262286  Primes whose binary expansion begins 100.
 * https://oeis.org/A262286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1679, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 70 terms through n = 9937 (2026-11-03)
 * Helpers:   bit_length, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate integers, test primality, keep those whose
   most‑significant three bits are 1,0,0 (binary starts 100). */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/bit_length.h"
#include "../utils/is_prime.h"
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
        unsigned bits = bit_length(p);

        /* check the leading 100 pattern */
        if (bits >= 3 && ((p >> (bits - 3)) == (seq_t)4))
            append_candidate(&cache, &cache_len, &cache_cap, p);
    }

    /* cache is zero‑based indexed */
    return cache[(size_t)n - 1];
}
