/*
 * A307585  Positive sums of two distinct cubes (of arbitrary sign).
 * https://oeis.org/A307585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A307585:
   the n‑th smallest positive integer that can be written as
   the sum of two distinct integer cubes (cubes may have arbitrary sign). */

/* ---------- standard headers ---------- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- utils from the framework ---------- */
#include "../utils/cmp_seq_t.h"          /* int cmp_seq_t(const void *, const void *); */
#include "../utils/append_candidate.h"   /* void append_candidate(seq_t **, size_t *, size_t *, seq_t); */

/* ---------- function declaration ---------- */
seq_t a(idx_t n)
{
    /* static cache of the ascending list of positive sums of two distinct cubes */
    static seq_t *sums   = NULL;   /* pointer to the unique sorted values          */
    static size_t sums_len = 0;     /* number of values stored                      */
    static bool computed = false;  /* flag whether the cache has been filled        */
    static const int B = 600;      /* search window: |a|, |b| <= B (positive case) */

    /* helper to build the list once */
    if (!computed)
    {
        /* dynamic array for intermediate sums (may contain duplicates) */
        seq_t *tmp   = NULL;       /* pointer to array of candidate values */
        size_t tmp_len = 0;        /* current number of candidates        */
        size_t tmp_cap = 0;        /* current capacity of tmp array       */

        /* enumerate all unordered pairs (a, b) with a < b, |a|,|b| <= B */
        for (int64_t a = -(int64_t)B; a <= (int64_t)B; ++a)
        {
            for (int64_t b = a + 1; b <= (int64_t)B; ++b)
            {
                /* cube of a and b as 64‑bit signed integers */
                int64_t cube_a = a * a * a;
                int64_t cube_b = b * b * b;
                int64_t sum    = cube_a + cube_b;

                if (sum > 0) {
                    /* cast to seq_t (works for signed < 0 as well) */
                    seq_t val = (seq_t)sum;
                    append_candidate(&tmp, &tmp_len, &tmp_cap, val);
                }
            }
        }

        /* sort the candidate list */
        if (tmp_len > 0)
            qsort(tmp, tmp_len, sizeof(seq_t), cmp_seq_t);

        /* remove duplicates in place, producing the final sorted unique list */
        if (tmp_len > 0) {
            size_t uniq = 1;
            for (size_t i = 1; i < tmp_len; ++i) {
                if (tmp[i] != tmp[i - 1])
                    tmp[uniq++] = tmp[i];
            }
            sums_len = uniq;
            sums     = tmp;              /* tmp now holds the unique list */
        } else {
            sums_len = 0;
            sums     = tmp;
        }

        computed = true;
    }

    /* n is 1‑based; if it is beyond the precomputed list we simply
       return the largest element available (not specified in the spec). */
    if (n == 0 || n > sums_len)
        return 0;
    return sums[n - 1];
}
