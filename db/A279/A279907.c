/*
 * A279907  Triangle read by rows: T(n,k) is the smallest power of n that is
 *          divisible by k, or -1 if no such power exists.
 * https://oeis.org/A279907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = T(i,j) where i = smallest row s.t. i*(i+1)/2 >= n j = n -
 *            i*(i-1)/2 T(i,j) = minimal e >= 0 with i^e divisible by j, or -1
 *            if none exist. For j=1, T(i,1)=0. For j>1, write j = ∏ p^α and i
 *            = ∏ p^β. If any prime p dividing j has β==0 then T(i,j) = -1.
 *            Otherwise T(i,j) = max_p ceil(α / β)
 * Range:     i8 n <= 9902, i16 n <= 9902, i32 n <= 9902, i64 n <= 9902,
 *            i128 n <= 9902, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute floor(sqrt(x)) for non‑negative 64‑bit x. */
static idx_t floor_sqrt(idx_t x)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Determine row i.  Find the smallest i such that i*(i+1)/2 >= n. */
    idx_t i;
    {
        idx_t d = floor_sqrt(1 + 8 * n);       /* sqrt(1+8n) <= 2^33 for n < 2^62 */
        i = (d - 1) / 2;
        if (i * (i + 1) / 2 < n)
            ++i;
    }

    /* Column j within this row. */
    idx_t prev = i * (i - 1) / 2;              /* cumulative count before row i */
    idx_t j = n - prev;                        /* 1 <= j <= i */

    /* Special case j == 1. */
    if (j == 1)
        return (seq_t)0;

    /* Factor j and compute valuations of i for each prime factor. */
    seq_t result = 0;
    idx_t jj = j;

    for (idx_t p = 2; p * p <= jj; ++p)
    {
        if (jj % p != 0)
            continue;

        /* exponent of p in j */
        idx_t exp_j = 0;
        while (jj % p == 0)
        {
            jj /= p;
            ++exp_j;
        }

        /* exponent of p in i */
        idx_t exp_i = 0;
        idx_t ii = i;
        while (ii % p == 0)
        {
            ii /= p;
            ++exp_i;
        }

        if (exp_i == 0)            /* prime factor of j not in i */
            return (seq_t)-1;

        seq_t need = (exp_j + exp_i - 1) / exp_i;   /* ceil(exp_j / exp_i) */
        if (need > result)
            result = need;
    }

    /* If jj > 1, it is a prime factor. */
    if (jj > 1)
    {
        idx_t p = jj;
        /* exponent of p in j is 1 */
        idx_t exp_i = 0;
        idx_t ii = i;
        while (ii % p == 0)
        {
            ii /= p;
            ++exp_i;
        }
        if (exp_i == 0)
            return (seq_t)-1;
        seq_t need = 1;                       /* ceil(1 / exp_i) == 1 */
        if (need > result)
            result = need;
    }

    return result;
}
