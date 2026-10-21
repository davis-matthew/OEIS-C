/*
 * A283635  Number of 2 X n 0..1 arrays with no 1 equal to more than one of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A283635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+6*a(n-2)-5*a(n-3)-42*a(n-4)-33*a(n-5)+51*a(n-6)+156*a(n-7)+144*a(n-8)+64*a(n-9)
 *            (valid for n ≥ 12) Initial values (offset 1): a(1)=0, a(2)=0,
 *            a(3)=0, a(4)=36, a(5)=88, a(6)=516, a(7)=2076, a(8)=7372,
 *            a(9)=27108, a(10)=92436, a(11)=308980
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t mul_uint(seq_t x, unsigned int m)
{
    seq_t r = 0;
    for (unsigned int i = 0; i < m; ++i)
        r += x;
    return r;
}

seq_t a(idx_t n)
{
    /* cached values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    /* base values */
    static const seq_t base[12] = {
        0,          /* dummy at 0 */
        0,          /* a(1) */
        0,          /* a(2) */
        0,          /* a(3) */
        36,         /* a(4) */
        88,         /* a(5) */
        516,        /* a(6) */
        2076,       /* a(7) */
        7372,       /* a(8) */
        27108,      /* a(9) */
        92436,      /* a(10) */
        308980      /* a(11) */
    };

    /* ensure cache memory */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed, abort safely */
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialize up to index 11 if needed */
    if (cache_cap <= 11) {
        for (size_t i = 1; i <= 11; i++)
            cache[i] = base[i];
    } else if (cache_cap > 11) {
        for (size_t i = 1; i <= 11; i++) {
            if (cache[i] == 0)   /* uninitialized slot */
                cache[i] = base[i];
        }
    }

    if ((idx_t)n <= 11)   /* return early for known part */
        return cache[n];

    /* coefficients (indices 0..8 correspond to a(n-1)..a(n-9)) */
    const int coeff[9] = {3, 6, -5, -42, -33, 51, 156, 144, 64};

    /* list of positive and negative coefficient indices */
    const int pos_idx[6] = {0, 1, 5, 6, 7, 8};
    const int neg_idx[3] = {2, 3, 4};

    for (idx_t i = 12; i <= n; ++i) {
        seq_t total = 0;
        /* add positive terms first */
        for (int k = 0; k < 6; ++k) {
            idx_t idx = i - 1 - pos_idx[k];
            seq_t term = cache[idx];
            total += mul_uint(term, (unsigned int)coeff[pos_idx[k]]);
        }
        /* subtract negative terms */
        for (int k = 0; k < 3; ++k) {
            idx_t idx = i - 1 - neg_idx[k];
            seq_t term = cache[idx];
            total -= mul_uint(term, (unsigned int)(-coeff[neg_idx[k]]));
        }
        cache[i] = total;
    }

    return cache[n];
}
