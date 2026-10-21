/*
 * A270757  Total sum of the sizes of all blocks with maximal element 3 in all
 *          set partitions of {1,2,...,n}.
 * https://oeis.org/A270757
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n-1) + 4*Bell(n-2) + 3*Bell(n-3) where Bell(k) is
 *            the k-th Bell number (Bell(0)=1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static void ensure_capacity(seq_t **ptr, size_t *cap, size_t needed)
{
    if (needed >= *cap) {
        size_t new_cap = (*cap > 0) ? (*cap) * 2 : 1;
        while (new_cap <= needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        *ptr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    /* static state --------------------------------------------------- */
    static seq_t *bell = NULL;      /* Bell numbers */
    static size_t bell_cap = 0;
    static idx_t  bell_last = 0;    /* largest k for which Bell(k) is known */

    static seq_t *a_vals = NULL;    /* a(n) values */
    static size_t a_cap = 0;
    static idx_t  a_last = 0;       /* largest n for which a(n) is known */

    /* initialise containers on first use --------------------------------- */
    if (!bell) {
        bell_cap = 1;
        bell = (seq_t *)malloc(bell_cap * sizeof(seq_t));
        if (!bell)
            exit(EXIT_FAILURE);
        bell[0] = (seq_t)1;          /* Bell(0) = 1 */
        bell_last = 0;
    }
    if (!a_vals) {
        a_cap = 3;
        a_vals = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (!a_vals)
            exit(EXIT_FAILURE);
        a_vals[0] = a_vals[1] = a_vals[2] = (seq_t)0;
        a_last = 2;
    }

    /* if we have not computed up to the requested n, extend our tables -- */
    if (n > a_last) {
        /* compute Bell numbers up to required index */
        idx_t need_B = n - 1;   /* we need Bell(n‑1) at least */
        if (need_B > bell_last) {
            for (idx_t m = bell_last + 1; m <= need_B; ++m) {
                ensure_capacity(&bell, &bell_cap, (size_t)m);
                seq_t sum = (seq_t)0;
                seq_t bin = (seq_t)1; /* C(m‑1,0) */
                for (idx_t k = 0; k < m; ++k) {
                    sum += bin * bell[(size_t)k];
                    if (k + 1 < m) {
                        /* bin = bin * (m‑1‑k) / (k+1) */
                        bin = bin * (seq_t)(m - 1 - k) / (seq_t)(k + 1);
                    }
                }
                bell[(size_t)m] = sum;
            }
            bell_last = need_B;
        }

        /* compute a(n) for all values up to the requested n */
        for (idx_t i = a_last + 1; i <= n; ++i) {
            ensure_capacity(&a_vals, &a_cap, (size_t)i);
            seq_t val = bell[(size_t)(i - 1)];
            val += (seq_t)4 * bell[(size_t)(i - 2)];
            val += (seq_t)3 * bell[(size_t)(i - 3)];
            a_vals[(size_t)i] = val;
        }
        a_last = n;
    }

    return a_vals[(size_t)n];
}
