/*
 * A322838  Number of positive integers less than n with more prime factors
 *          than n, counted with multiplicity.
 * https://oeis.org/A322838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ m < n : bigomega(m) > bigomega(n) }
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A322838 as described in OEIS */

/* Build smallest prime factor table up to n */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

static idx_t   a_cap = 0;        /* size of a_arr               */
static seq_t *a_arr = NULL;     /* cached sequence values       */

/* Recompute the whole sequence up to limit (inclusive) */
static void recompute_up_to(idx_t limit)
{
    if (limit == 0) return;

    /* allocate array for Omega values (bigomega) */
    seq_t *omega = (seq_t*)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!omega) return;                      /* allocation failed */

    /* build smallest prime factor table */
    idx_t *spf;
    build_spf(limit, &spf);

    /* compute bigomega for each integer */
    omega[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
    {
        uint64_t x = i;
        seq_t  cnt = 0;
        while (x > 1) {
            x /= spf[x];
            ++cnt;
        }
        omega[i] = cnt;
    }
    free(spf);

    /* allocate new a_arr */
    seq_t *new_a = (seq_t*)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!new_a) {
        free(omega);
        return;
    }

    /* frequency table for Omega values seen so far */
    seq_t freq[64] = {0};

    /* compute a[n] sequentially */
    for (idx_t n = 1; n <= limit; ++n)
    {
        seq_t w = omega[n];
        seq_t cnt = 0;
        for (int k = (int)w + 1; k < 64; ++k)
            cnt += freq[k];
        new_a[n] = cnt;
        freq[(int)w]++;           /* include n for future positions */
    }

    /* free old array and replace */
    free(a_arr);
    a_arr = new_a;
    a_cap = limit;

    free(omega);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined for n >= 1, but guard against 0 */
    if (n > a_cap) recompute_up_to(n);
    return a_arr[n];
}
