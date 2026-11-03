/*
 * A379332  Numbers k such that k^sigma(k) == k (mod sigma(k)).
 * https://oeis.org/A379332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that k^σ(k) ≡ k (mod σ(k))
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by brute‑force testing integers in ascending
   order, using the provided utilities for σ(n) (sigma_of) and modular
   exponentiation (mod_pow). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* static cache for the found terms */
    static seq_t *terms    = NULL;
    static size_t  term_count = 0;
    static size_t  term_cap   = 0;

    /* next integer to test */
    static idx_t next_k = 1;

    /* extend the cache until we have at least n terms */
    while (term_count < n) {
        seq_t k      = (seq_t)next_k;          /* current candidate */
        seq_t sigma  = sigma_of(next_k);       /* σ(k) */

        /* modular exponentiation: k^σ(k) mod σ(k) */
        seq_t rem = powmod(k, (uint64_t)sigma, sigma);

        /* k mod σ(k) */
        seq_t modk = k % sigma;

        if (rem == modk) {
            /* add to cache */
            if (term_count == term_cap)
                grow_seq(&terms, &term_cap, term_count + 1);
            terms[term_count++] = k;
        }
        ++next_k;
    }

    /* return the requested term (offset 1) */
    return terms[n - 1];
}
