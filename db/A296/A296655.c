/*
 * A296655  Numbers k such that phi(x) = k has a positive even number of
 *          solutions.
 * https://oeis.org/A296655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that the equation φ(x) = k has an even
 *            positive number of solutions
 * Range:     u8 none, u16 none, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds φ(x) for all x ≤ N, counts the multiplicity of each value k,
   and collects the k's with positive even multiplicity.  The needed bound N is grown
   until at least n values have been collected. */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for the generated sequence. */
static seq_t *seq_cache = NULL;   /* Array of results */
static size_t seq_len = 0;        /* Number of stored terms */
static size_t seq_cap = 0;        /* Allocated capacity */

/* Ensure that seq_cache contains at least 'need' terms. */
static void ensure_sequence(idx_t need)
{
    if (seq_len >= need) return;          /* Already have enough */

    idx_t limit = 250000;                  /* Initial bound; adjust if necessary */

    /* Rebuild until we have enough terms. */
    while (true) {

        /* Allocate arrays for φ(x) and counters. */
        seq_t *phi = (seq_t *)malloc(sizeof(seq_t) * (limit + 1));
        if (!phi) return;                 /* Out of memory */
        size_t *cnt = (size_t *)calloc(limit + 1, sizeof(size_t));
        if (!cnt) { free(phi); return; }

        /* Initialise φ[i] = i */
        for (idx_t i = 0; i <= limit; ++i) {
            phi[i] = (seq_t)i;
        }

        /* Sieve to compute φ for all i. */
        for (idx_t i = 2; i <= limit; ++i) {
            if (phi[i] == (seq_t)i) {               /* i is prime */
                for (idx_t j = i; j <= limit; j += i) {
                    phi[j] -= phi[j] / (seq_t)i;
                }
            }
        }

        /* Count occurrences of each φ value. */
        for (idx_t x = 1; x <= limit; ++x) {
            seq_t k = phi[x];
            if ((idx_t)k <= limit)              /* k always ≤ x ≤ limit */
                ++cnt[(size_t)k];
        }

        /* Free φ array now that counting is done. */
        free(phi);

        /* Build the new sequence cache. */
        free(seq_cache);
        seq_cache = NULL;
        seq_len = 0;
        seq_cap = 0;

        for (idx_t k = 1; k <= limit; ++k) {
            if (cnt[(size_t)k] && (cnt[(size_t)k] & 1ULL) == 0) {
                /* Append k to seq_cache, growing if necessary. */
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap ? seq_cap * 2 : 128;
                    seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
                }
                seq_cache[seq_len++] = (seq_t)k;
            }
        }

        free(cnt);

        if (seq_len >= need) break;        /* Enough terms collected */

        /* Not enough terms: double the search bound and try again. */
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* Indexing starts at 1; 0 is unused */
    ensure_sequence(n);
    return seq_cache[n - 1];
}
