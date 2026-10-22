/*
 * A398996  The binomial matrix of the Motzkin numbers. Square array read by
 *          ascending antidiagonals.
 * https://oeis.org/A398996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A398996 a(n) is the (i,j)-entry of the binomial matrix of *
 *            Motzkin numbers read by descending i along each antidiagonal: *
 *            * a(n) = A(i,j) where i+j = d and i = d - p, j = p, * with d
 *            chosen such that (d+1)(d+2)/2 > n. * * A(i,j) = Σ_{m=0..|i-j|}
 *            binomial(|i-j|,m) * s^(|i-j|-m) * M(min(i,j)+m) *...
 * Range:     u8 n <= 20, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, coords_from_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"          /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/coords_from_n.h"

/* ------------------------------------------------------------------ */
/*  Helper: Motzkin numbers M(n)                                      */
/* ------------------------------------------------------------------ */

static seq_t *motzkin = NULL;   /* cache of computed Motzkin numbers     */
static idx_t  motzkin_len = 0;  /* number of cached elements             */

static void ensure_motzkin(idx_t n)
{
    if (motzkin_len > n)
        return; /* already enough */

    /* Allocate a new array large enough */
    idx_t new_len = n + 1;
    seq_t *new_arr = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
    if (!new_arr) exit(1); /* out of memory */

    /* Copy already computed values */
    if (motzkin) {
        memcpy(new_arr, motzkin, (size_t)motzkin_len * sizeof(seq_t));
        free(motzkin);
    }

    /* Compute the new entries (indices >= motzkin_len) */
    for (idx_t i = motzkin_len; i < new_len; ++i) {
        if (i == 0) {
            new_arr[0] = (seq_t)1;
        } else if (i == 1) {
            new_arr[1] = (seq_t)1;
        } else {
            seq_t sum = new_arr[i - 1];            /* first term M(i-1) */
            for (idx_t k = 0; k <= i - 2; ++k)
                sum += new_arr[k] * new_arr[i - 2 - k];
            new_arr[i] = sum;
        }
    }

    motzkin     = new_arr;
    motzkin_len = new_len;
}

static inline seq_t motzkin_at(idx_t n) { return motzkin[n]; }

/* ------------------------------------------------------------------ */
/*  Helper: Coordinates (i,j) from the antidiagonal index n            */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Helper: Evaluate A(i,j) from the definition                        */
/* ------------------------------------------------------------------ */

static seq_t A_value(idx_t i, idx_t j)
{
    idx_t d = (i > j) ? i - j : j - i;   /* |i-j|          */
    idx_t p = (i < j) ? i : j;           /* min(i,j)       */

    /* Sum over m = 0..d of binomial(d,m) * s^(d-m) * M(p+m)   */
    if (i >= j) { /* s = +1 */
        seq_t acc = 0;
        for (idx_t m = 0; m <= d; ++m)
            acc += binomial(d, m) * motzkin_at(p + m);
        return acc;
    } else { /* s = -1 */
        seq_t pos = 0, neg = 0;
        for (idx_t m = 0; m <= d; ++m) {
            seq_t term = binomial(d, m) * motzkin_at(p + m);
            if ((d - m) & 1) /* odd -> negative */
                neg += term;
            else
                pos += term;
        }
        return pos - neg;
    }
}

/* ------------------------------------------------------------------ */
/*  Public function: a(n)                                            */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    idx_t i, j;
    coords_from_n(n, &i, &j);

    /* Motzkin numbers up to max(i,j) are needed. */
    ensure_motzkin((i > j) ? i : j);

    return A_value(i, j);
}
