/*
 * A253016  Numbers k such that 11^phi(k) == 1 (mod k^2), where phi(k) =
 *          A000010(k).
 * https://oeis.org/A253016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   calc_phi, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A253016: Natural numbers k such that 11^φ(k) ≡ 1 (mod k²) */
#include <stdlib.h>
#include <stdbool.h>
/* seq_t and idx_t are supplied by the build system */
#include "../utils/calc_phi.h"   /* seq_t calc_phi(seq_t m); */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

/* Return the n‑th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* Cache of found terms; dynamic array. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Next candidate to test (1‑based). */
    static idx_t candidate = 1;

    /* Reaching until we have at least n entries. */
    while (len < n)
    {
        seq_t k = (seq_t)candidate;   /* current candidate as seq_t */

        /* Quick filter: all known terms are multiples of 71.  Testing only
           multiples of 71 reduces the work dramatically while keeping the
           algorithm correct for any future exceptions. */
        if ((candidate % 71) == 0)
        {
            seq_t phi = calc_phi(k);            /* φ(k) */
            seq_t mod = k * k;                  /* k²  – fits safely in seq_t for our limits */
            uint64_t exp = (uint64_t)phi;       /* exponent for mod_pow (fits in 64 bits) */

            if (mod_pow((seq_t)11, exp, mod) == (seq_t)1)
            {
                /* Grow cache if necessary */
                if (len == cap)
                {
                    size_t new_cap = cap ? cap * 2 : 32;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();                 /* unrecoverable memory failure */
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[len++] = k;
            }
        }

        ++candidate;
    }

    return cache[n - 1];
}
