/*
 * A360891  G.f. satisfies A(x) = 1 + x/(1 - x^4) * A(x/(1 - x^4)).
 * https://oeis.org/A360891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊(n‑1)/4⌋} binomial(n‑1‑3k, k) * a(n‑1‑4k), with
 *            a(0)=1
 * Range:     u8 n <= 13, u16 n <= 20, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence given in sequence A360891. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"     /* provides binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Static cache for computed values. */
    static seq_t  *vals   = NULL;  /* array of values */
    static unsigned char *done = NULL;  /* 0 = not computed, 1 = computed */
    static size_t cap = 0;        /* current allocation size */

    /* Expand storage to include index n if necessary. */
    if (n >= cap) {
        size_t new_cap = (n + 1);
        if (new_cap < 2 * cap) {
            new_cap = 2 * cap;
        }
        if (new_cap < 1) {
            new_cap = 1;
        }

        /* realloc with explicit cast so it compiles in C++ */
        seq_t *new_vals = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
        unsigned char *new_done = (unsigned char *)realloc(done,
                                   new_cap * sizeof(unsigned char));
        if (!new_vals || !new_done) {
            /* Out of memory – abort the program. */
            exit(EXIT_FAILURE);
        }
        vals = new_vals;
        done = new_done;

        /* Initialize new slots as not computed. */
        for (size_t i = cap; i < new_cap; ++i) {
            done[i] = 0;
        }
        cap = new_cap;
    }

    /* Base case. */
    if (n == 0) {
        if (!done[0]) {
            vals[0] = (seq_t)1;
            done[0] = 1;
        }
        return vals[0];
    }

    /* Return memoized value if already computed. */
    if (done[n]) {
        return vals[n];
    }

    /* Compute via the recurrence. */
    seq_t sum = (seq_t)0;
    idx_t maxk = (n - 1) / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t sub_n = n - 1 - 4 * k;        /* n‑1‑4k */
        idx_t bin_n = n - 1 - 3 * k;        /* n‑1‑3k */
        seq_t bin   = binomial(bin_n, k);   /* binomial(n‑1‑3k, k) */
        seq_t term  = bin * a(sub_n);       /* multiplication in seq_t */
        sum += term;
    }

    vals[n] = sum;
    done[n] = 1;
    return sum;
}
