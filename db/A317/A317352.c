/*
 * A317352  G.f. satisfies: A(x) = 1 + x*A(x) * ( d/dx x*A(x)^2 ).
 * https://oeis.org/A317352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A317352
   Formula: a(0)=1
            a(1)=1
            for n>=2:
              a(n) = Σ_{i=0}^{n-1} a(i)·E(n-1-i) + 2·Σ_{k=1}^{n-1} k·a(k)·E(n-1-k)
   where E(m) = Σ_{i=0}^{m} a(i)·a(m-i)   (coefficient of x^m in A(x)^2)
*/

#include <stdlib.h>

static seq_t *a_arr = NULL;   /* coefficients a(n) */
static seq_t *E_arr = NULL;   /* convolution array E(m) = sum a(i)a(m-i) */
static idx_t  cap   = 0;      /* allocated capacity */
static idx_t  size  = 0;      /* number of computed terms */

/* enlarge space to hold at least 'need' elements */
static void ensure_capacity(idx_t need)
{
    if (need <= cap)
        return;
    idx_t new_cap = (cap == 0) ? 16 : cap << 1;
    while (new_cap < need)
        new_cap <<= 1;

    if (a_arr == NULL) {
        a_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        E_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        E_arr = (seq_t *)realloc(E_arr, new_cap * sizeof(seq_t));
    }
    /* initialize newly allocated slots to zero */
    for (idx_t i = cap; i < new_cap; ++i) {
        a_arr[i] = 0;
        E_arr[i] = 0;
    }
    cap = new_cap;
}

/* public function */
seq_t a(idx_t n)
{
    if (n >= size) {
        ensure_capacity(n + 1);

        if (size == 0) {          /* initialise base case a(0) */
            a_arr[0] = 1;
            E_arr[0] = 1;        /* E(0) = a0*a0 */
            size = 1;
        }

        for (idx_t m = size; m <= n; ++m) {
            if (m == 0) {
                /* a(0) already known */
                continue;
            }

            /* compute a(m) for m>=1 */
            if (m == 1) {
                a_arr[1] = 1;  /* from the functional equation */
            } else {
                /* compute Σ_{i=0}^{m-1} a(i)·E(m-1-i) */
                seq_t c3 = 0;
                idx_t idx = m - 1;
                for (idx_t i = 0; i <= idx; ++i) {
                    c3 += a_arr[i] * E_arr[idx - i];
                }

                /* compute t_{m-2} = Σ_{k=1}^{m-1} k·a(k)·E(m-1-k) */
                seq_t t = 0;
                idx_t p = m - 2;   /* p = m-2 */
                for (idx_t k = 1; k <= p + 1; ++k) {
                    t += E_arr[p - k + 1] * ((seq_t)k) * a_arr[k];
                }

                a_arr[m] = c3 + (2 * t);
            }

            /* after a(m) is known, compute E(m) = Σ_{i=0}^{m} a(i)·a(m-i) */
            seq_t em = 0;
            for (idx_t i = 0; i <= m; ++i) {
                em += a_arr[i] * a_arr[m - i];
            }
            E_arr[m] = em;
        }

        size = n + 1;
    }

    return a_arr[n];
}
