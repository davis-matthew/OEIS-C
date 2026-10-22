/*
 * A338663  a(n) = Sum_{d|n} (n/d)^n * binomial(d+n/d-1, d).
 * https://oeis.org/A338663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n/d)^n * binomial(d + n/d - 1, d)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"


/* Enumerate all positive divisors of n.  Caller must free  *out. */
static void get_divisors(idx_t n, idx_t **out, size_t *cnt)
{
    size_t sz = 16;
    idx_t *divs = (idx_t *)malloc(sz * sizeof(idx_t));
    size_t m   = 0;

    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            if (m == sz)
            {
                sz <<= 1;
                divs = (idx_t *)realloc(divs, sz * sizeof(idx_t));
            }
            divs[m++] = i;
            if (i != n / i)
            {
                if (m == sz)
                {
                    sz <<= 1;
                    divs = (idx_t *)realloc(divs, sz * sizeof(idx_t));
                }
                divs[m++] = n / i;
            }
        }
    }

    *out = divs;
    *cnt = m;
}

/* Main sequence function, external linkage. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    size_t div_cnt;
    idx_t *divs;
    get_divisors(n, &divs, &div_cnt);

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < div_cnt; ++i)
    {
        idx_t d = divs[i];
        idx_t m = n / d;               /* m = n/d */

        seq_t term1 = big_pow((seq_t)m, (seq_t)n);          /* (n/d)^n */
        idx_t a_param = d + m - 1;
        seq_t term2 = binomial(a_param, d);                /* binom(d+m-1, d) */
        sum += term1 * term2;
    }

    free(divs);
    return sum;
}
