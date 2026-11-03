/*
 * A355430  Primes starting with an even decimal digit.
 * https://oeis.org/A355430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime whose reversal is even (i.e., whose first
 *            decimal digit is even)
 * Range:     u8 n <= 18, u16 n <= 2945, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9010 (2026-11-03)
 * Helpers:   is_prime, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/reverse_digits_idx.h"
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
    /* Cache of previous terms */
    static seq_t *cache = NULL;      /* array of terms */
    static size_t cap   = 0;         /* capacity of cache */
    static size_t len   = 0;         /* number of terms stored */
    static idx_t next_trial = 2;    /* next integer to test */

    /* Search for new terms until we have at least n of them */
    idx_t cand;
    for (cand = next_trial; len < n; ++cand) {
        if (!is_prime(cand))
            continue;
        /* rev(cand) returns the decimal reversal of cand */
        /* If the reversal is even, the first decimal digit of cand is even */
        if ((reverse_digits_idx(cand) & 1) != 0)
            continue;
        /* Append the prime as the next term */
        append_candidate(&cache, &len, &cap, (seq_t)cand);
    }

    /* Record the next integer to start searching from */
    next_trial = cand;

    /* cache[n‑1] now holds the requested term */
    return cache[n - 1];
}
