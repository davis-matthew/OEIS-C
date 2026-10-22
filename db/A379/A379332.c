/*
 * A379332  Numbers k such that k^sigma(k) == k (mod sigma(k)).
 * https://oeis.org/A379332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that k^σ(k) ≡ k (mod σ(k))
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   grow_seq, mod_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by brute‑force testing integers in ascending
   order, using the provided utilities for σ(n) (sigma_of) and modular
   exponentiation (mod_pow). */

#include <stddef.h>
#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/mod_pow.h"
#include "../utils/grow_seq.h"

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
        seq_t rem = mod_pow(k, (uint64_t)sigma, sigma);

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
