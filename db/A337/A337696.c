/*
 * A337696  Numbers k such that the k-th composition in standard order
 *          (A066099) is strict and pairwise non-coprime, meaning the parts
 *          are distinct and any two of them have a common divisor > 1.
 * https://oeis.org/A337696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A337696
   a(n) = k such that the k‑th composition in standard order (A066099)
   is strict (all parts distinct, each part > 1) and pairwise non‑coprime */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache for previous results */
    static seq_t *cache = NULL;      /* array of found k values */
    static size_t cache_len = 0;     /* number of cached results */
    static size_t cache_cap = 0;     /* allocated capacity of cache */
    static idx_t next_k = 0;         /* next k to test (starts at 0) */

    /* initialise cache lazily */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure – unlikely */
    }

    /* produce results until we have at least n entries */
    while (cache_len < n) {
        idx_t k = next_k;          /* candidate index */
        unsigned long long bits = k;
        unsigned long long bitpos = 1;   /* 1‑based bit positions */
        unsigned long long prev = 0;
        unsigned long long part[64];
        size_t m = 0;

        /* build the composition parts from the set bits of k */
        while (bits) {
            if (bits & 1ULL) {
                unsigned long long p = bitpos - prev;
                part[m++] = p;
                prev = bitpos;
            }
            ++bitpos;
            bits >>= 1;
        }

        bool ok = true;

        /* empty composition (k = 0) is accepted */
        if (m == 0) {
            ok = true;
        } else {
            /* all parts must be > 1 */
            for (size_t i = 0; i < m; ++i) {
                if (part[i] <= 1) {
                    ok = false;
                    break;
                }
            }

            /* parts must be distinct */
            if (ok) {
                for (size_t i = 0; i < m && ok; ++i) {
                    for (size_t j = i + 1; j < m; ++j) {
                        if (part[i] == part[j]) {
                            ok = false;
                            break;
                        }
                    }
                }
            }

            /* pairwise gcd must be > 1 */
            if (ok) {
                for (size_t i = 0; i < m && ok; ++i) {
                    seq_t ai = (seq_t)part[i];
                    for (size_t j = i + 1; j < m; ++j) {
                        seq_t gj = gcd(ai, (seq_t)part[j]);
                        if (gj == (seq_t)1) {
                            ok = false;
                            break;
                        }
                    }
                }
            }
        }

        if (ok) {
            /* store the qualifying k */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (tmp == NULL) { free(cache); cache = NULL; return 0; } /* fail */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)k;
        }

        ++next_k;  /* advance to next candidate */
    }

    return cache[n - 1];
}
