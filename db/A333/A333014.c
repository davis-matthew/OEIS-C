/*
 * A333014  Numbers which can written in exactly four ways as a sum of two
 *          distinct nonzero pentagonal numbers.
 * https://oeis.org/A333014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers which can be written in exactly four ways as a sum of
 *            two distinct nonzero pentagonal numbers. (Pentagonal number P(k)
 *            = k*(3*k-1)/2.)
 * Range:     u8 none, u16 none, u32 n <= 5000, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* The types seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* static data for memoisation */
    static seq_t  *terms     = NULL;
    static size_t  terms_cap = 0;
    static size_t  terms_len = 0;

    /* If we already have the requested term, return it. */
    if (terms_len < (size_t)n) {
        /* initial bound that is expected to contain enough terms */
        idx_t bound = 20000000ULL;

        /* Re‑compute until we have at least n terms. */
        while (terms_len < (size_t)n) {
            /* Reset the previously allocated term array. */
            free(terms);
            terms     = NULL;
            terms_cap = 0;
            terms_len = 0;

            /* ----------------------------------------------------------
               1. Compute all pentagonal numbers needed
               ---------------------------------------------------------- */
            size_t max_k = 0;
            for (size_t k = 1; ; ++k) {
                seq_t pk = ((seq_t)k) * (3 * k - 1) / 2;
                if (pk > bound - 1) break;   /* P(k) + P(1) would exceed bound */
                ++max_k;
            }
            seq_t *pent = (seq_t *)malloc((max_k + 1) * sizeof(seq_t));
            if (!pent) return 0;                /* out of memory */
            pent[0] = 0;                         /* unused */
            for (size_t k = 1; k <= max_k; ++k)
                pent[k] = ((seq_t)k) * (3 * k - 1) / 2;

            /* ----------------------------------------------------------
               2. Count unordered pairs of pentagonal numbers
               ---------------------------------------------------------- */
            uint16_t *cnt = (uint16_t *)calloc((size_t)bound + 1,
                                               sizeof(uint16_t));
            if (!cnt) { free(pent); return 0; }

            for (size_t k = 2; k <= max_k; ++k) {
                seq_t pk = pent[k];
                for (size_t l = 1; l < k; ++l) {
                    seq_t sum = pk + pent[l];
                    if (sum > bound) break;   /* further l will only increase sum */
                    uint16_t v = cnt[(size_t)sum];
                    if (v < 5) ++cnt[(size_t)sum];   /* keep values ≤5 */
                }
            }

            /* ----------------------------------------------------------
               3. Collect values that occur exactly four times
               ---------------------------------------------------------- */
            for (size_t s = 0; s <= (size_t)bound; ++s) {
                if (cnt[s] == 4) {
                    if (terms_len == terms_cap) {
                        size_t new_cap = terms_cap ? terms_cap * 2 : 128;
                        seq_t *tmp = (seq_t *)realloc(terms,
                                                      new_cap * sizeof(seq_t));
                        if (!tmp) { free(pent); free(cnt); return 0; }
                        terms     = tmp;
                        terms_cap = new_cap;
                    }
                    terms[terms_len++] = (seq_t)s;
                }
            }

            free(pent);
            free(cnt);

            /* If we still need more terms, increase the bound and try again. */
            if (terms_len < (size_t)n) {
                bound *= 2;
                /* Stop if the bound would overflow.  In practice this
                   never happens with the required range.  */
                if (bound > ((idx_t)~0ULL) / 2ULL) break;
            }
        }
    }

    /* The requested term is now guaranteed to be present. */
    return terms[(size_t)(n - 1)];
}
