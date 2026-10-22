/*
 * A369116  Expansion of (1 - x)^2 * Sum_{j>=0} (x^j / (1 - Sum_{k=1..j}
 *          x^k)).
 * https://oeis.org/A369116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(n) - 2*b(n-1) + b(n-2) where b(n) is the coefficient of
 *            Σ_{j≥0} (x^j / (1 - Σ_{k=1..j} x^k)); a(0) = 1, a(1) = -1, a(2)
 *            = 1, a(3) = 0, ... (see OEIS A369116)
 * Range:     i8 n <= 12, i16 n <= 21, i32 n <= 37, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *alloc_array(size_t n)
{
#ifndef __cplusplus
    return (seq_t *)calloc(n, sizeof(seq_t));
#else
    return new seq_t[n]();
#endif
}
static void free_array(seq_t *p)
{
#ifndef __cplusplus
    free(p);
#else
    delete[] p;
#endif
}

static seq_t *resize_array(seq_t *old, size_t old_n, size_t new_n)
{
    seq_t *res = alloc_array(new_n);
    if (old)
    {
        for (size_t i = 0; i < old_n; ++i)
            res[i] = old[i];
        free_array(old);
    }
    return res;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;                  /* safe conversion for loops */

    static size_t cap = 0;                 /* current allocation size     */
    static seq_t *B = NULL;                /* coefficients of b(n)         */
    static seq_t *A = NULL;                /* result a(n)                  */
    static size_t computed_max = 0;        /* number of indices computed   */

    /* recompute everything if a larger n is requested */
    if (N + 1 > computed_max)
    {
        size_t new_cap = N + 1;

        /* resize arrays */
        B = resize_array(B, cap, new_cap);
        A = resize_array(A, cap, new_cap);
        cap = new_cap;

        /* initialise B to zero */
        for (size_t i = 0; i < new_cap; ++i)
            B[i] = 0;

        /* temporary array for compositions counts */
        seq_t *f = alloc_array(cap);
        for (size_t j = 0; j <= N; ++j)
        {
            size_t M = N - j;                 /* maximum m for this j          */
            f[0] = 1;                        /* C_j(0) = 1 (empty composition) */
            B[j] += f[0];                    /* add contribution for m = 0      */

            seq_t window = 0;                 /* running sum of last j f's       */
            for (size_t m = 1; m <= M; ++m)
            {
                window += f[m - 1];
                if (m - 1 >= j)               /* drop old term if window too large */
                    window -= f[m - 1 - j];
                f[m] = window;                /* C_j(m) = window                */
                B[j + m] += f[m];             /* add to b(j+m)                   */
            }
        }
        free_array(f);

        /* compute a(n) from b(n) */
        A[0] = B[0];                         /* a_0 = b_0                      */
        for (size_t i = 1; i <= N; ++i)
        {
            seq_t val = B[i];
            val -= (seq_t)2 * B[i - 1];
            if (i >= 2)
                val += B[i - 2];
            A[i] = val;
        }

        computed_max = N + 1;                /* indices up to N are ready */
    }

    return A[N];
}
