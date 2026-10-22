/*
 * A398145  Number of rooted binary simplex time-consistent leaf-labeled
 *          galled trees with n leaves.
 * https://oeis.org/A398145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recursion derived from the EGF * A(x) = x +
 *            (1/2)*A(x)^2 + (x/2)*(A(x)/(1-A(x)))^2 * Let a(n) be the numbers
 *            and c(n) the coefficients of A/(1-A). * Recurrence for n ≥ 2: *
 *            S1 = Σ_{k=1}^{n-1} binom(n,k) a(k) a(n-k) * S2 = Σ_{k=1}^{n-1}
 *            binom(n-1,k) c(k) c(n-1-k) * a(n) = (S1 +...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
static seq_t* alloc_arr(idx_t sz) { return new seq_t[sz]; }
static void free_arr(seq_t* p) { delete[] p; }
#else
static seq_t* alloc_arr(idx_t sz) { return (seq_t*)calloc((size_t)sz, sizeof(seq_t)); }
static void free_arr(seq_t* p) { free(p); }
#endif

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 0)
    {
#if defined(__cplusplus)
        return seq_t(0);
#else
        return 0;
#endif
    }
    /* Allocate arrays for a[0..n] and c[0..n] */
    seq_t *a_arr = alloc_arr(n + 1);
    seq_t *c_arr = alloc_arr(n + 1);

    /* Initial values */
    a_arr[0] = 0;
    c_arr[0] = 0;
    a_arr[1] = 1;
    c_arr[1] = 1;

    /* Compute values up to n */
    for (idx_t cur = 2; cur <= n; ++cur)
    {
        /* --- Compute S1 and S2 for a[cur] --- */
        seq_t S1 = 0;
        seq_t S2 = 0;
        seq_t bin_n  = (seq_t)cur;          /* binom(cur,1)   */
        seq_t bin_n1 = (seq_t)(cur - 1);    /* binom(cur-1,1) */

        for (idx_t k = 1; k <= cur - 1; ++k)
        {
            seq_t prodA = a_arr[k] * a_arr[cur - k];
            S1 += bin_n * prodA;

            seq_t prodC = c_arr[k] * c_arr[cur - 1 - k];
            S2 += bin_n1 * prodC;

            if (k < cur - 1)
            {
                /* binom(cur,k+1) = binom(cur,k) * (cur-k)/(k+1) */
                bin_n = (bin_n * (seq_t)(cur - k)) / (seq_t)(k + 1);

                /* binom(cur-1,k+1) = binom(cur-1,k) * (cur-1-k)/(k+1) */
                bin_n1 = (bin_n1 * (seq_t)(cur - 1 - k)) / (seq_t)(k + 1);
            }
        }
        seq_t nS2 = S2 * (seq_t)cur;
        seq_t numerator = S1 + nS2;
        a_arr[cur] = numerator / 2;

        /* --- Compute c[cur] --- */
        seq_t sum_c = 0;
        bin_n = (seq_t)cur;   /* binom(cur,1) again for this sum */

        for (idx_t k = 1; k <= cur - 1; ++k)
        {
            seq_t prod = a_arr[k] * c_arr[cur - k];
            sum_c += bin_n * prod;

            if (k < cur - 1)
            {
                bin_n = (bin_n * (seq_t)(cur - k)) / (seq_t)(k + 1);
            }
        }
        c_arr[cur] = a_arr[cur] + sum_c;
    }

    /* Prepare return value and free memory */
    seq_t result = a_arr[n];
    free_arr(a_arr);
    free_arr(c_arr);
    return result;
}
