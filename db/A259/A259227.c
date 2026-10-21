/*
 * A259227  Hydropronic numbers: numbers n that can be written as a product of
 *          2 integers whose sum is equal to ceiling(n/ceiling(sqrt(n))) +
 *          ceiling(sqrt(n)).
 * https://oeis.org/A259227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1189, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by testing integers for the hydropronic property. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Returns 1 if m is a hydropronic number, 0 otherwise. */
static bool is_hydropronic(idx_t m)
{
    seq_t mm = (seq_t)m;                /* convert to seq_t for arithmetic */

    /* ceil_sqrt(m) */
    seq_t sq = floor_sqrt(mm);          /* floor sqrt of m */
    seq_t sq_sq = sq * sq;
    if (sq_sq < mm) {
        sq++;                           /* ceil sqrt */
    }

    /* ceil(m / ceil_sqrt) */
    seq_t ceil_div = (mm + sq - 1) / sq;/* equivalent to integer ceil division */

    seq_t S = ceil_div + sq;           /* target sum */

    seq_t SS = S * S;                  /* S^2 */
    seq_t FM = mm << 2;                /* 4*m */

    /* No solution if S^2 < 4m (underflow would otherwise occur) */
    if (SS < FM) {
        return false;
    }

    seq_t D = SS - FM;                 /* discriminant S^2 - 4m */

    seq_t sqrt_D = floor_sqrt(D);
    if (sqrt_D * sqrt_D != D) {        /* discriminant not a perfect square */
        return false;
    }

    /* a = (S + sqrt_D)/2, b = S - a must satisfy a*b == m */
    if (((S + sqrt_D) & 1) != 0) {     /* parity check */
        return false;
    }

    seq_t a = (S + sqrt_D) / 2;
    seq_t b = S - a;

    return (a * b == mm);
}

/* Cached sequence data */
static seq_t *hydropronic = NULL;
static size_t hydro_len = 0;
static size_t hydro_cap = 0;
static idx_t next_candidate = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1 */

    /* Grow cache to at least n terms */
    while (hydro_len < n) {
        if (is_hydropronic(next_candidate)) {
            /* store the new term */
            if (hydro_len >= hydro_cap) {
                /* increase capacity: at least 16, then double */
                hydro_cap = hydro_cap ? hydro_cap * 2 : 16;
                hydropronic = (seq_t*)realloc(hydropronic, hydro_cap * sizeof(seq_t));
            }
            hydropronic[hydro_len++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }

    return hydropronic[n - 1];
}
