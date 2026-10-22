/*
 * A367485  Expansion of e.g.f. 1/(3 - 2*exp(x))^(x/2).
 * https://oeis.org/A367485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (1/2) * Sum_{k=1..n} A367489(k) *
 *            binomial(n-1,k-1) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types */
extern seq_t a(idx_t n);

static idx_t  max_n = 0;          /* highest n already computed   */
static seq_t *a_arr = NULL;       /* array of a(n) values         */
static seq_t *b_arr = NULL;       /* array of A367489(k) values   */

seq_t a(idx_t n)
{
    /* First call: allocate and initialise base cases  */
    if (!a_arr) {
        a_arr = (seq_t *)malloc(2 * sizeof(seq_t));
        b_arr = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!a_arr || !b_arr) exit(1);

        a_arr[0] = (seq_t)1;          /* a(0) = 1     */
        a_arr[1] = (seq_t)0;          /* placeholder  */
        b_arr[0] = (seq_t)0;          /* unused       */
        b_arr[1] = (seq_t)0;          /* b(1) = 0     */

        /* compute a(1) from the recurrence */
        {
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;               /* C(0,0)           */
            seq_t term  = b_arr[1];
            term *= binom;
            term *= a_arr[0];
            sum += term;
            a_arr[1] = sum / (seq_t)2;
        }
        max_n = 1;
    }

    /* If the requested n has already been computed, return it */
    if (n <= max_n)
        return a_arr[n];

    /* Extend the storage to accommodate up to n */
    size_t new_cap = (size_t)n + 1;
    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    b_arr = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
    if (!a_arr || !b_arr) exit(1);

    /* Compute values for all m from max_n+1 up to n */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        /* ---- compute A367489(m) ---- */
        if (m >= 2) {
            seq_t numerator = (seq_t)2 * (m - 1) * m;   /* 2*(m‑1)*m          */
            seq_t binom     = (seq_t)1; /* C(m,0)                */

            /* iterate j = 1 .. m‑1 to build C(m,j) */
            for (idx_t j = 1; j < m; ++j) {
                binom = binom * (m - j + 1) / j;   /* C(m,j)          */

                if (j >= 2) {                     /* need j = t+1   */
                    idx_t t   = j - 1;             /* t = j‑1        */
                    seq_t term = b_arr[j];        /* b[t+1]          */
                    term = term * (seq_t)(2 * t) * binom;
                    numerator += term;
                }
            }
            b_arr[m] = numerator / (seq_t)(m - 1);
        } else {
            b_arr[m] = (seq_t)0;
        }

        /* ---- compute a(m) ---- */
        seq_t sum   = (seq_t)0;
        seq_t binomA = (seq_t)1;                      /* C(m‑1,0)            */

        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = b_arr[k];
            term *= binomA;
            term *= a_arr[m - k];
            sum += term;

            if (k < m)
                binomA = binomA * (m - k) / k;       /* C(m‑1,k)        */
        }
        a_arr[m] = sum / (seq_t)2;
    }

    max_n = n;
    return a_arr[n];
}
