/*
 * A328056  Numbers k such that phi(k) > phi(k+1) > phi(k+2) where phi is the
 *          Euler totient function (A000010).
 * https://oeis.org/A328056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that phi(k) > phi(k+1) > phi(k+2)
 * Range:     u8 none, u16 n <= 654, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9861 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/totient.h"
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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t tested_k = 0;   /* last k that was examined */

    if (n == (idx_t)0) return 0;  /* undefined for n=0, return 0 */

    while (cache_len < (size_t)n) {
        ++tested_k;                 /* next k to check */

        seq_t phi_k   = totient(tested_k);
        seq_t phi_k1  = totient(tested_k + 1);
        seq_t phi_k2  = totient(tested_k + 2);

        if (phi_k > phi_k1 && phi_k1 > phi_k2) {
            append_candidate(&cache, &cache_len, &cache_cap,
                             (seq_t)tested_k);
        }
    }
    return cache[(size_t)(n - 1)];
}
