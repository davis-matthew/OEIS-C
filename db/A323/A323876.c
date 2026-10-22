/*
 * A323876  Number of labeled graphs on n nodes with three connected
 *          components.
 * https://oeis.org/A323876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A143543(n+1, 3)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   grow_table, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of labeled graphs on n nodes with exactly 3 connected components */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/grow_table.h"

/* ------------------------------------------------------------------ */
/*  Static tables: connected graphs (c[n]), graphs with 2 components (a2[n]),
    graphs with 3 components (a3[n]), and pre‑computed powers of two (pow2_t[n]) */

static seq_t *c = NULL;          /*  c[0] unused, c[1] = 1 */
static size_t c_cap = 0;

static seq_t *a2 = NULL;         /*  a2[0] = 0 */
static size_t a2_cap = 0;

static seq_t *a3 = NULL;         /*  a3[0] = 0 */
static size_t a3_cap = 0;

static seq_t *pow2_t = NULL;     /*  pow2_t[t] = 2^{t(t-1)/2} */
static size_t pow2_t_cap = 0;

static size_t max_computed = 0;  /* Highest n whose entries are filled */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Compute tables up to index N (inclusive).  This routine is called
    by the public function a(idx_t n). */
static void compute_up_to(size_t N)
{
    if (N <= max_computed)
        return;

    /* enlarge tables if necessary */
    grow_table(&c,    &c_cap,    N + 1);
    grow_table(&a2,   &a2_cap,   N + 1);
    grow_table(&a3,   &a3_cap,   N + 1);
    grow_table(&pow2_t, &pow2_t_cap, N + 1);

    /* pre‑compute powers of two that will be needed for connected graphs */
    for (size_t t = max_computed; t < N + 1; ++t)
    {
        idx_t exp = (idx_t)t * (t - 1) / 2;
        pow2_t[t] = pow2(exp);              /* seq_t pow2(idx_t) from utils */
    }

    /* fill new entries */
    for (size_t n = max_computed + 1; n <= N; ++n)
    {
        /* --- connected graphs c[n] ------------------------------------- */
        seq_t total = pow2_t[n];            /* 2^{n(n-1)/2}              */
        seq_t sum   = 0;
        seq_t binom = (seq_t)1;             /* C(n-1,0)                   */
        for (size_t i = 1; i <= n - 1; ++i)
        {
            seq_t term = binom * c[i];
            term *= pow2_t[n - i];          /* 2^{(n-i)(n-i-1)/2}       */
            sum += term;
            if (i < n - 1)                 /* update binom to C(n-1,i)   */
            {
                seq_t tmp = binom;
                tmp *= (seq_t)(n - i);
                tmp /= (seq_t)i;
                binom = tmp;
            }
        }
        c[n] = total - sum;                /* number of connected graphs */

        /* --- graphs with 2 components a2[n] ----------------------------- */
        seq_t sum2 = 0;
        if (n >= 2)
        {
            binom = (seq_t)1;
            for (size_t i = 1; i <= n - 1; ++i)
            {
                seq_t term = binom * c[i] * c[n - i];
                sum2 += term;
                if (i < n - 1)
                {
                    seq_t tmp = binom;
                    tmp *= (seq_t)(n - i);
                    tmp /= (seq_t)i;
                    binom = tmp;
                }
            }
        }
        a2[n] = sum2;

        /* --- graphs with 3 components a3[n] ----------------------------- */
        seq_t sum3 = 0;
        if (n >= 3)
        {
            binom = (seq_t)1;
            for (size_t i = 1; i <= n - 2; ++i)
            {
                seq_t term = binom * c[i] * a2[n - i];
                sum3 += term;
                if (i < n - 2)
                {
                    seq_t tmp = binom;
                    tmp *= (seq_t)(n - i);
                    tmp /= (seq_t)i;
                    binom = tmp;
                }
            }
        }
        a3[n] = sum3;
    }

    max_computed = N;
}

/* ------------------------------------------------------------------ */
/*  Public entry point required by the build system.                     */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    compute_up_to((size_t)n);
    return a3[n];
}
