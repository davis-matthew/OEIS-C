/*
 * A356246  Primes whose reversal is a multiple of 14.
 * https://oeis.org/A356246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   is_prime, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime p such that rev(p) is a multiple of 14.
   It searches successive primes and keeps a static cache of qualifying
   primes. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 1;  // number up to which we have examined

    /* extend cache until it contains at least n elements */
    while (cache_len < n) {
        idx_t cand = last_checked + 1;
        /* search for the next prime that satisfies the condition */
        while (true) {
            if (is_prime(cand)) {
                idx_t revcand = reverse_digits_idx(cand);
                if (revcand % 14 == 0) {
                    seq_t val = (seq_t)cand;
                    append_candidate(&cache, &cache_len, &cache_cap, val);
                    break;
                }
            }
            cand++;
        }
        last_checked = cand;
    }
    return cache[n - 1];
}
