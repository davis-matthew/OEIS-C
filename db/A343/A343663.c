/*
 * A343663  Number of unlabeled binary rooted semi-identity plane trees with
 *          2*n - 1 nodes.
 * https://oeis.org/A343663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = 1 + x + x*(A(x)^2 - A(x^2)); a(n) = coefficient of xⁿ⁻¹
 *            in A(x)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 37, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 1, a(3) = 2, a(4) = 4, a(5) = 12, … (OEIS A343663) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values of the series A(x) = Σ c_k x^k  (c_0 = 1, c_1 = 1, …)   */
    static seq_t *c = NULL;          /* array of c_k values */
    static size_t cap = 0;           /* current allocation size */
    static idx_t last = (idx_t)-1;   /* highest index that has been computed;
                                      (idx_t)-1 means nothing yet */

    if (n == 0) return (seq_t)0; /* offset 1; undefined for n<1 but harmless */

    idx_t required = n - 1;        /* we need c_{required} */

    /* Ensure enough space.  If growing, the new slots will be written later. */
    if ((size_t)(required + 1) > cap)
    {
        size_t new_cap = (required + 1) * 3 / 2;   /* 1.5× growth */
        if (new_cap <= (size_t)(required + 1))
            new_cap = required + 1;

        seq_t *tmp = (seq_t *)realloc(c, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        c = tmp;
        cap = new_cap;
    }

    /* Compute any missing coefficients up to the required index. */
    while ((idx_t)last + 1 <= required)
    {
        ++last;                     /* next index to compute */

        if (last == 0)
        {
            c[0] = (seq_t)1;
            continue;
        }

        if (last == 1)
        {
            c[1] = (seq_t)1;
            continue;
        }

        /* c_last = sum_{i=0}^{last-1} c_i * c_{last-1-i}
           minus c_{(last-1)/2} if (last-1) is even. */
        seq_t sum = (seq_t)0;
        idx_t m = last - 1;

        for (idx_t i = 0; i <= m; ++i)
        {
            sum += c[i] * c[m - i];
        }

        if ((m & 1u) == 0u)   /* if m is even, subtract the middle term */
        {
            idx_t half = m >> 1;
            sum -= c[half];
        }

        c[last] = sum;
    }

    return c[required];
}
