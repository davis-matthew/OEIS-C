/*
 * A296727  Expansion of e.g.f. arcsinh(x)/(1 - x).
 * https://oeis.org/A296727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * a(n-1) + n! * c_n, where c_n = 0 if n is even, else
 *            c_n = (-1)^m * ((2m)!)^2 / (4^m * (m!)^2) with m = (n-1)/2. This
 *            recurrence allows an O(n) computation of a(n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t a_val = 0;             /* a(0) */
    /* variables for computing the odd‑index correction term */
    seq_t curT = 1;              /* T_m = ((2m)!)^2 / (4^m * (m!)^2)   */
    idx_t  curM = 0;             /* current m for which curT is valid   */

    for (idx_t i = 1; i <= n; ++i)
    {
        if ((i & 1) == 0)        /* even i: a(i) = i * a(i-1) */
        {
            a_val = a_val * (seq_t)i;
        }
        else                    /* odd i: use the recurrence with T_m */
        {
            idx_t targetM = (i - 1) >> 1;

            /* advance curM up to targetM, updating curT */
            while (curM < targetM)
            {
                idx_t prevM = curM;

                /* R = (2*prevM+1)*(2*prevM+2)/(prevM+1)  -- integer */
                seq_t R = (seq_t)(2 * prevM + 1) *
                          (seq_t)(2 * prevM + 2);
                R /= (seq_t)(prevM + 1);

                /* Update curT:   T_new = T_prev * R^2 / 4   */
                seq_t R_sq = R * R;
                curT = curT * R_sq;
                curT /= (seq_t)4;

                curM++;
            }

            seq_t prod = a_val * (seq_t)i;
            if ((curM & 1) == 0)      /* m even: add T */
                a_val = prod + curT;
            else                     /* m odd: subtract T */
                a_val = prod - curT;
        }
    }
    return a_val;
}
