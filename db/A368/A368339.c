/*
 * A368339  Take the solution to Pellian equation x^2 - 8*n*y^2 = 1 with
 *          smallest positive y and x >= 0; sequence gives a(n) = y, or 0 if n
 *          is twice a positive square. A368340 gives values of x.
 * https://oeis.org/A368339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002349(8*n) Computes the smallest positive y that
 *            satisfies x^2 - 8*n*y^2 = 1, or returns 0 when n is twice a
 *            positive square. Uses the continued‑fraction method for the Pell
 *            equation
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* For very small n we can early‑exit. */
    if (n == 0) return (seq_t)0;

    /* D = 8*n  (fits in seq_t for the tested ranges). */
    seq_t D_seq = (seq_t)n * (seq_t)8;
    uint64_t D = (uint64_t)D_seq;          /* For period computation. */

    /* Check for square D: this corresponds to n being twice a square. */
    seq_t sqrt_seq = floor_sqrt(D_seq);
    if (sqrt_seq * sqrt_seq == D_seq) return (seq_t)0;

    uint64_t a0 = (uint64_t)sqrt_seq;

    /* Build one period of the continued fraction expansion of sqrt(D). */
    uint64_t period[2000];
    size_t period_len = 0;

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    do
    {
        m = d * a - m;
        uint64_t num = D - m * m;            /* num = D - m^2, always divisible by d. */
        d = num / d;
        a = (a0 + m) / d;
        period[period_len++] = a;
    }
    while (a != 2 * a0 && period_len < 2000);

    /* Convergents:  use recurrence h_i = a_i*h_{i-1}+h_{i-2},
       k_i = a_i*k_{i-1}+k_{i-2}  (x and y). */
    seq_t h_prev2 = 0, h_prev1 = 1;
    seq_t k_prev2 = 1, k_prev1 = 0;

    /* First convergent uses a0. */
    seq_t h_curr = (seq_t)a0 * h_prev1 + h_prev2;
    seq_t k_curr = (seq_t)a0 * k_prev1 + k_prev2;
    if ((h_curr * h_curr) - D_seq * k_curr * k_curr == 1) return k_curr;

    /* Shift for subsequent terms. */
    h_prev2 = h_prev1; h_prev1 = h_curr;
    k_prev2 = k_prev1; k_prev1 = k_curr;

    /* Iterate over the periodic part cyclically until the Pell
       equation is satisfied. */
    for (size_t idx = 0; ; idx++)
    {
        uint64_t ai = period[idx % period_len];
        h_curr = (seq_t)ai * h_prev1 + h_prev2;
        k_curr = (seq_t)ai * k_prev1 + k_prev2;
        if ((h_curr * h_curr) - D_seq * k_curr * k_curr == 1)
            return k_curr;

        h_prev2 = h_prev1; h_prev1 = h_curr;
        k_prev2 = k_prev1; k_prev1 = k_curr;
    }
}
