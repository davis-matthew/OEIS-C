/*
 * A303354  Expansion of Product_{k>0} (1+k^2*x^k)^(-1/k).
 * https://oeis.org/A303354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] ∏_{k>0} (1 + k^2 · x^k)^(-1/k) Let d(m) = m·c(m)
 *            where c(m) = -∑_{d|m} (-1)^(d-1) (m/d)^(2d)/m. Then d(m) =
 *            -∑_{d|m} (-1)^(d-1) (m/d)^(2d) Recurrence: a(0) = 1 a(n) = (1/n)
 *            · ∑_{m=1}^n d(m) · a(n-m) for n ≥ 1 The computation proceeds by
 *            first pre‑computing d(m) for all m ≤ n using di...
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 29, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* static cache for the largest n seen so far */
    static bool init = false;
    static idx_t max_n = 0;
    static seq_t *d_arr = NULL;   /* d(m) for m=1..max_n */
    static seq_t *a_arr = NULL;   /* a(n) for n=0..max_n */

    /* recompute everything if we are asked for a larger n */
    if (!init || n > max_n)
    {
        idx_t new_max = n;

        /* grow or allocate the arrays */
        d_arr = (seq_t *)realloc(d_arr, (new_max + 1) * sizeof(seq_t));
        a_arr = (seq_t *)realloc(a_arr, (new_max + 1) * sizeof(seq_t));

        /* initialise with zeros (d_arr[0] unused, a_arr[0] set later) */
        for (idx_t i = 1; i <= new_max; ++i)
            d_arr[i] = 0;

        /* compute d(m) = -∑_{d|m} (-1)^(d-1) (m/d)^(2d) */
        for (idx_t d = 1; d <= new_max; ++d)
        {
            /* (-1)^(d-1) : odd d -> +1, even d -> -1 */
            seq_t sign = (d & 1) ? 1 : -1;

            /* iterate over multiples m = d·t, t = 1,2,... */
            for (idx_t m = d, t = 1; m <= new_max; m += d, ++t)
            {
                seq_t pow = big_pow((seq_t)t, (seq_t)(2 * d)); /* t^(2d) */
                seq_t term = pow;
                if (sign == -1)
                    term = -term;        /* term = (-1)^(d-1) * t^(2d) */
                d_arr[m] -= term;          /* accumulate with minus sign */
            }
        }

        /* compute a(n) by the convolution recurrence */
        a_arr[0] = 1;
        for (idx_t i = 1; i <= new_max; ++i)
        {
            seq_t sum = 0;
            for (idx_t m = 1; m <= i; ++m)
                sum += d_arr[m] * a_arr[i - m];
            a_arr[i] = sum / (seq_t)i;  /* division is exact */
        }

        max_n = new_max;
        init   = true;
    }

    return a_arr[n];
}
