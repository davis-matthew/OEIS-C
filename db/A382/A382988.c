/*
 * A382988  a(n) is the total sum of semiperimeters over all
 *          (>=,>=)-polyominoes of length n.
 * https://oeis.org/A382988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A382988
   a(n) = (3*f_{n+2} - 4*f_{n+1} - 5*f_n) / 2
   where f_n are defined by the series 1 / sqrt(1 - 2*x - 3*x^2)
   and satisfy   s_n = 2*s_{n-1} + 3*s_{n-2} ,  s_0 = 1 , s_1 = 2
   and   f_n = (s_n - sum_{i=1}^{n-1} f_i*f_{n-i}) / 2
*/

#include <stdlib.h>

/* Internal static data */
static seq_t *f_vals = NULL;      /* f[0 .. ]            */
static seq_t *s_vals = NULL;      /* s[0 .. ]            */
static size_t  cap   = 0;         /* current capacity    */
static idx_t   n_done = 0;        /* highest index computed +1 */

/* Ensure internal arrays are large enough */
static void ensure_capacity(idx_t needed)
{
    if (cap > (size_t)needed) return;
    size_t newcap = cap ? cap << 1 : 16;
    while (newcap <= (size_t)needed) newcap <<= 1;

    seq_t *newf = (seq_t *)malloc(newcap * sizeof(seq_t));
    seq_t *news = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!newf || !news) abort();

    for (size_t i = 0; i < cap; ++i) {
        newf[i] = f_vals[i];
        news[i] = s_vals[i];
    }
    for (size_t i = cap; i < newcap; ++i) {
        newf[i]  = 0;
        news[i]  = 0;
    }

    free(f_vals);
    free(s_vals);
    f_vals = newf;
    s_vals = news;
    cap = newcap;
}

/* Compute f and s up to index upto   */
static void compute_terms(idx_t upto)
{
    if (n_done > upto) return;

    ensure_capacity(upto);

    /* initialise first terms if needed */
    if (n_done == 0) {
        f_vals[0] = (seq_t)1;
        s_vals[0] = (seq_t)1;
        /* s_1 will be set when we reach index 1 */
        n_done = 1;
    }

    for (idx_t j = n_done; j <= upto; ++j) {
        /* compute s_j */
        if (j == 1) {
            s_vals[1] = (seq_t)2;
        } else if (j >= 2) {
            s_vals[j] = 2 * s_vals[j-1] + 3 * s_vals[j-2];
        }

        /* sum_{i=1}^{j-1} f_i * f_{j-i}  */
        seq_t sum = (seq_t)0;
        idx_t half = j / 2;
        if (half > 0) {
            if (j & 1) { /* odd j */
                for (idx_t i = 1; i <= half; ++i) {
                    seq_t prod = f_vals[i] * f_vals[j-i];
                    sum += prod + prod;   /* 2 * prod */
                }
            } else { /* even j */
                /* middle term */
                seq_t mid = f_vals[half];
                sum += mid * mid;
                for (idx_t i = 1; i < half; ++i) {
                    seq_t prod = f_vals[i] * f_vals[j-i];
                    sum += prod + prod;   /* 2 * prod */
                }
            }
        }

        /* compute f_j */
        f_vals[j] = (s_vals[j] - sum) / (seq_t)2;
    }
    n_done = upto + 1;
}

/* Public entry point */
seq_t a(idx_t n)
{
    /* need f_{n}, f_{n+1}, f_{n+2} */
    compute_terms(n + 2);

    idx_t i = n;
    seq_t fn   = f_vals[i];
    seq_t fn_1 = f_vals[i+1];
    seq_t fn_2 = f_vals[i+2];

    /* numerator = 3*fn_2 - 4*fn_1 - 5*fn ; non‑negative */
    seq_t num  = 3 * fn_2;
    seq_t sum_terms = 4 * fn_1;
    sum_terms += 5 * fn;
    if (num < sum_terms) num = 0;        /* safety – should not happen */
    else num -= sum_terms;

    return num / (seq_t)2;
}
