/*
 * A247172  Expansion of -1 - (sqrt(x^4 + 4*x^3 - 2*x^2 - 4*x + 1) - x^2 - 2*x
 *          - 1)/(4*x).
 * https://oeis.org/A247172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence from OEIS (valid for n >= 5) (n+1)*a(n) +
 *            2*(-2*n+1)*a(n-1) + 2*(-n+2)*a(n-2) + 2*(2*n-7)*a(n-3) +
 *            (n-5)*a(n-4) = 0. Initial values: a(0)=0, a(1)=1, a(2)=1,
 *            a(3)=3, a(4)=9
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /*  Static cache of previously computed terms  */
    static seq_t *cache = NULL;
    static size_t size = 0;          /* number of terms stored */

    /* Resize cache if necessary */
    if (n + 1 > size)
    {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)  /* allocation failed – return zero (should never happen) */
            return (seq_t)0;
        cache = tmp;

        /* Fill in new entries */
        for (size_t i = size; i < new_size; ++i)
        {
            idx_t idx = (idx_t)i;
            if (idx == 0)
                cache[i] = (seq_t)0;
            else if (idx == 1 || idx == 2)
                cache[i] = (seq_t)1;
            else if (idx == 3)
                cache[i] = (seq_t)3;
            else if (idx == 4)
                cache[i] = (seq_t)9;
            else
            {
                /* Recurrence for n >= 5 using only unsigned arithmetic */
                seq_t a1 = cache[idx - 1];
                seq_t a2 = cache[idx - 2];
                seq_t a3 = cache[idx - 3];
                seq_t a4 = cache[idx - 4];

                seq_t coeff1 = ((seq_t)4 * idx) - (seq_t)2; /* 4*n-2   */
                seq_t coeff2 = ((seq_t)2 * idx) - (seq_t)4; /* 2*n-4   */
                seq_t coeff3 = ((seq_t)4 * idx) - (seq_t)14;/* 4*n-14  */
                seq_t coeff4 = idx - (seq_t)5;              /* n-5     */

                seq_t pos = coeff1 * a1;
                pos += coeff2 * a2;

                seq_t neg = coeff3 * a3;
                neg += coeff4 * a4;

                seq_t numerator = pos - neg;
                seq_t denominator = idx + (seq_t)1;

                cache[i] = numerator / denominator;
            }
        }
        size = new_size;
    }

    return cache[n];
}
