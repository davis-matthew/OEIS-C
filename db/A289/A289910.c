/*
 * A289910  The denominator of r(n), where r(n) = 1/r(n-2) + r(n-1);
 *          r(1)=r(2)=1/2.
 * https://oeis.org/A289910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: r(1)=r(2)=1/2, r(n)=1/r(n-2)+r(n-1);
 * a(n) is the denominator of r(n) in lowest terms.
 * a(1)=a(2)=2. */

#include <stdlib.h>

/* Euclidean GCD for seq_t */
static seq_t local_gcd(seq_t a, seq_t b)
{
    while (b)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* static memoisation arrays */
    static seq_t *num = NULL;
    static seq_t *den = NULL;
    static idx_t last = 0;   /* largest index computed */
    static idx_t cap  = 0;   /* array capacity (1‑based) */

    if (n == 0)
        return (seq_t)0; /* undefined but harmless */

    /* grow cache if needed */
    if (n >= cap)
    {
        size_t new_cap = (size_t)n + 1; /* index 0 unused */
        if (num == NULL)
        {
            num = (seq_t *)calloc(new_cap, sizeof(seq_t));
            den = (seq_t *)calloc(new_cap, sizeof(seq_t));
        }
        else
        {
            num = (seq_t *)realloc(num, new_cap * sizeof(seq_t));
            den = (seq_t *)realloc(den, new_cap * sizeof(seq_t));
        }
        /* zero new cells to avoid undefined values if accessed before set */
        for (size_t i = cap; i < new_cap; ++i)
        {
            num[i] = 0;
            den[i] = 0;
        }
        cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        if (i == 1 || i == 2)
        {
            num[i] = (seq_t)1;
            den[i] = (seq_t)2;
        }
        else
        {
            seq_t a_im2 = num[i - 2];   /* numerator i-2 */
            seq_t b_im2 = den[i - 2];   /* denominator i-2 */
            seq_t a_im1 = num[i - 1];   /* numerator i-1 */
            seq_t b_im1 = den[i - 1];   /* denominator i-1 */

            seq_t num_raw = b_im2 * b_im1 + a_im1 * a_im2;
            seq_t den_raw = a_im2 * b_im1;

            seq_t g = local_gcd(num_raw, den_raw);
            if (g != (seq_t)1)
            {
                num_raw /= g;
                den_raw /= g;
            }

            num[i] = num_raw;
            den[i] = den_raw;
        }
    }

    last = n;
    return den[n];
}
