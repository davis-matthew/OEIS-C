/*
 * A282972  Number of ways to write n as 4*x^4 + y^2 + z^2 + w^2 with x,y,z,w
 *          nonnegative integers such that 79*y^2 - 220*y*z + 205*z^2 is a
 *          square.
 * https://oeis.org/A282972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to n = 4*x^4 + y^2 + z^2 + w^2 with
 *            x,y,z,w ≥ 0 integers and 79*y^2 - 220*y*z + 205*z^2 a perfect
 *            square
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, four_x4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/four_x4.h"

/* Static table of precomputed values up to MAX_N. */
/* MAX_N is chosen large enough for typical tests (≈2·10⁴). */
#define MAX_N 20000

static seq_t *sums = NULL;          /* array of counts, indexed by n */
static bool initialized = false;

/* ------------------------------------------------------------------------
   Fill the static array 'sums' with counts up to max_n (inclusive).
   ------------------------------------------------------------------------ */
static void compute_sums(idx_t max_n)
{
    /* Allocate or reallocate the table */
    if (sums == NULL) {
        sums = (seq_t *)malloc((max_n + 1) * sizeof(seq_t));
    } else {
        sums = (seq_t *)realloc(sums, (max_n + 1) * sizeof(seq_t));
    }
    /* Initialise to zero */
    for (idx_t i = 0; i <= max_n; ++i) sums[i] = 0;

    /* Pre‑compute limits */
    seq_t max_n_seq = (seq_t)max_n;

    /* ymax and xmax */
    idx_t ymax = (idx_t)floor_sqrt(max_n_seq);
    /* Find xmax such that 4*x^4 <= max_n */
    idx_t xmax = 0;
    for (idx_t x = 1;; ++x) {
        seq_t v = four_x4(x);
        if (v > max_n_seq) break;
        xmax = x;
    }

    /* List of squares up to max_n for quick w² generation */
    size_t wcnt = ymax + 1;          /* w ranges 0..ymax */
    seq_t *w_sqr = (seq_t *)malloc(wcnt * sizeof(seq_t));
    for (idx_t w = 0; w < wcnt; ++w) w_sqr[w] = (seq_t)w * (seq_t)w;

    /* Core triple loop over x,y,z */
    for (idx_t x = 0; x <= xmax; ++x) {
        seq_t addx = four_x4(x);
        seq_t xsq = (seq_t)x * (seq_t)x;
        seq_t xx4 = xsq * xsq;

        for (idx_t y = 0; y <= ymax; ++y) {
            seq_t ysq = (seq_t)y * (seq_t)y;
            for (idx_t z = 0; z <= ymax; ++z) {
                seq_t zsq = (seq_t)z * (seq_t)z;

                /* Compute expression 79*y^2 - 220*y*z + 205*z^2 */
                seq_t expr = ((seq_t)79) * ysq
                          - ((seq_t)220) * ( (seq_t)y * (seq_t)z )
                          + ((seq_t)205) * zsq;

                /* Check if expr is a perfect square */
                seq_t k = floor_sqrt(expr);
                if (k * k != expr) continue;

                /* Base sum without w: s = addx + ysq + zsq */
                seq_t s = addx + ysq + zsq;
                if (s > max_n_seq) continue;

                /* Remaining part to be filled by w^2 */
                seq_t remain = max_n_seq - s;
                idx_t wmax = (idx_t)floor_sqrt(remain);
                for (idx_t w = 0; w <= wmax; ++w) {
                    seq_t total = s + w_sqr[w];
                    /* total <= max_n_seq by construction */
                    sums[(idx_t)total] = sums[(idx_t)total] + 1;
                }
            }
        }
    }

    free(w_sqr);
}

/* ------------------------------------------------------------------------
   Main function required by OEIS: a(idx_t n)
   ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) {
        compute_sums(MAX_N);
        initialized = true;
    }
    /* If n may exceed the precomputed range, extend the table once */
    if (n > MAX_N) {
        /* Compute a larger table; use the requested n as new limit */
        compute_sums(n);
    }
    return sums[(idx_t)n];
}
