/*
 * A245859  Number of preferential arrangements of n labeled elements such
 *          that the minimal number of elements per rank equals 6.
 * https://oeis.org/A245859
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A245732(n,6) - A245732(n,7) A245732(n,m) counts
 *            preferential arrangements of n labeled elements with each rank
 *            containing at least m elements. It satisfies the convolution
 *            recurrence g_m[0] = 1, g_m[n] = 0 for n < m, g_m[n] = Σ_{k=m..n}
 *            binom(n,k) * g_m[n-k] for n ≥ m The implementation co...
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 21, u64 n <= 30, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#  include <algorithm>
#endif

static seq_t *g6 = NULL;           /* coefficients for m = 6   */
static seq_t *g7 = NULL;           /* coefficients for m = 7   */
static idx_t  cap = 0;             /* current allocation size */

/* Ensure that coefficients are computed up to index n. */
static void
ensure_up_to(idx_t n)
{
    if (n < cap) return;               /* already available */

    idx_t new_cap = n + 1;

    /* Allocate new arrays */
    seq_t *new_g6;
    seq_t *new_g7;
#ifdef __cplusplus
    new_g6 = new seq_t[new_cap];
    new_g7 = new seq_t[new_cap];
#else
    new_g6 = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    new_g7 = (seq_t *)malloc(sizeof(seq_t) * new_cap);
#endif
    if (!new_g6 || !new_g7) abort();

    /* Copy old data */
    if (g6) {
#ifdef __cplusplus
        std::copy(g6, g6 + cap, new_g6);
        std::copy(g7, g7 + cap, new_g7);
        delete[] g6;
        delete[] g7;
#else
        memcpy(new_g6, g6, sizeof(seq_t) * cap);
        memcpy(new_g7, g7, sizeof(seq_t) * cap);
        free(g6);
        free(g7);
#endif
    }

    /* Initialise new entries to 0 */
    for (idx_t i = cap; i < new_cap; ++i) {
        new_g6[i] = 0;
        new_g7[i] = 0;
    }

    /* Base case */
    if (cap == 0) {
        new_g6[0] = 1;
        new_g7[0] = 1;
    }

    /* Compute coefficients for i = cap .. n */
    for (idx_t i = (cap == 0 ? 1 : cap); i < new_cap; ++i) {
        /* Compute g6[i] */
        if (i < 6) {
            new_g6[i] = 0;
        } else {
            seq_t sum = 0;
            idx_t k = 6;
            seq_t bin = 1;
            /* binom(i,6) */
            for (idx_t t = 1; t <= 6; ++t) {
                bin = bin * (i - 6 + t) / t;
            }
            while (k <= i) {
                sum += bin * new_g6[i - k];
                if (k < i) {
                    /* binom(i,k+1) = binom(i,k) * (i-k)/(k+1) */
                    bin = bin * (i - k) / (k + 1);
                }
                ++k;
            }
            new_g6[i] = sum;
        }

        /* Compute g7[i] */
        if (i < 7) {
            new_g7[i] = 0;
        } else {
            seq_t sum = 0;
            idx_t k = 7;
            seq_t bin = 1;
            /* binom(i,7) */
            for (idx_t t = 1; t <= 7; ++t) {
                bin = bin * (i - 7 + t) / t;
            }
            while (k <= i) {
                sum += bin * new_g7[i - k];
                if (k < i) {
                    bin = bin * (i - k) / (k + 1);
                }
                ++k;
            }
            new_g7[i] = sum;
        }
    }

    /* Replace old arrays */
    g6 = new_g6;
    g7 = new_g7;
    cap = new_cap;
}

/* External function to return the sequence value integrated with memoisation. */
seq_t a(idx_t n)
{
    if (n < 6) return 0;       /* undefined below the offset – return 0 */
    ensure_up_to(n);
    return g6[n] - g7[n];
}
