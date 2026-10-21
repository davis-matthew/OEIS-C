/*
 * A284267  Number of terms with coefficient 1 in the Stern polynomial
 *          B(2n+1,x): a(n) = A056169(A277324(n)).
 * https://oeis.org/A284267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  Number of terms with coefficient 1 in the Stern polynomial B(2n+1,x)
   Computed by building the coefficients of B(k) for all k ≤ 2n+1 and
   counting how many of them equal 1.  See OEIS A284267 for details. */

#include <stdlib.h>
#include <stdint.h>  /* for uint64_t */

#define COEFF_WIDTH 64   /* enough to store a coefficient of any B(k)  */

static seq_t *coeffs = NULL;   /* flattened 2‑D array: coeffs[k*COEFF_WIDTH + d] */
static idx_t *degs   = NULL;   /* degree of B(k) */
static idx_t  max_k  = 0;      /* maximum k for which B(k) is known */

/* Ensure the static arrays can hold data up to index `need`. */
static void ensure_capacity(idx_t need)
{
    if (need <= max_k)
        return;

    idx_t new_max = need;

    /* Reallocate coefficient array */
    seq_t *new_coeffs = (seq_t *)realloc(coeffs,
                         (new_max + 1) * COEFF_WIDTH * sizeof(seq_t));
    if (!new_coeffs) abort();
    coeffs = new_coeffs;

    /* Reallocate degree array */
    idx_t *new_degs = (idx_t *)realloc(degs, (new_max + 1) * sizeof(idx_t));
    if (!new_degs) abort();
    degs = new_degs;

    /* Compute B(k) for all newly required indices. */
    for (idx_t k = max_k + 1; k <= new_max; ++k)
    {
        if (k == 0)   /* B(0) = 0, degree 0 */
        {
            coeffs[0 * COEFF_WIDTH + 0] = 0;
            degs[0] = 0;
        }
        else if (k == 1)   /* base case B(1) = 1 */
        {
            coeffs[1 * COEFF_WIDTH + 0] = 1;
            degs[1] = 0;
        }
        else if ((k & 1) == 0) /* even: k = 2*m, B(k) = x * B(m) */
        {
            idx_t m   = k >> 1;
            idx_t dmp = degs[m];
            /* shift coefficients left by one */
            coeffs[k * COEFF_WIDTH + 0] = 0;
            for (idx_t d = 0; d <= dmp; ++d)
                coeffs[k * COEFF_WIDTH + (d + 1)] = coeffs[m * COEFF_WIDTH + d];
            degs[k] = dmp + 1;
        }
        else /* odd: k = 2*m+1, B(k) = B(m) + B(m+1) */
        {
            idx_t m      = k >> 1;
            idx_t m1     = m + 1;
            idx_t dm     = degs[m];
            idx_t dm1    = degs[m1];
            idx_t dmax   = (dm > dm1) ? dm : dm1;
            for (idx_t d = 0; d <= dmax; ++d)
            {
                seq_t val = 0;
                if (d <= dm)
                    val += coeffs[m * COEFF_WIDTH + d];
                if (d <= dm1)
                    val += coeffs[m1 * COEFF_WIDTH + d];
                coeffs[k * COEFF_WIDTH + d] = val;
            }
            degs[k] = dmax;
        }
    }

    max_k = new_max;
}

seq_t a(idx_t n)
{
    /* Index of the Stern polynomial we need: k = 2*n + 1. */
    idx_t k = (n << 1) + 1;

    ensure_capacity(k);

    /* Count how many coefficients equal to 1. */
    idx_t d   = degs[k];
    seq_t cnt = 0;
    for (idx_t i = 0; i <= d; ++i)
        if (coeffs[k * COEFF_WIDTH + i] == 1)
            ++cnt;
    return cnt;
}
