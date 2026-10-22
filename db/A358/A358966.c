/*
 * A358966  a(n) = n!*Sum_{m=1..floor(n/2)} 1/(m*binomial(n-1,2*m-1)*n).
 * https://oeis.org/A358966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*Sum_{m=1..floor(n/2)} 1/(m*binomial(n-1,2*m-1)*n)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = Σ_{m=1..⌊n/2⌋} ((2m-1)! * (n-2m)!)/m */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    idx_t max_m = n / 2;

    /* Branch for 64‑bit builds to avoid overflow using 128‑bit arithmetic */
    if (sizeof(seq_t) <= 8)
    {
        unsigned __int128 t = 1;          /* term for current m */
        unsigned __int128 sum = 0;

        /* t initially equals (n-2)! */
        for (idx_t k = 1; k <= n - 2; ++k)
            t *= (unsigned __int128)k;

        for (idx_t m = 1; m <= max_m; ++m)
        {
            sum += t;                 /* add current term */

            if (m == max_m)
                break;                /* last term, no update needed */

            /* Update t to next term using recurrence:
               t_{m+1} = t_m * (2m)*(2m+1)*m / ((n-2m-1)*(n-2m)*(m+1)) */
            t *= (unsigned __int128)(2 * m);
            t *= (unsigned __int128)(2 * m + 1);
            t *= (unsigned __int128)m;
            t /= (unsigned __int128)(n - 2 * m - 1);
            t /= (unsigned __int128)(n - 2 * m);
            t /= (unsigned __int128)(m + 1);
        }

        return (seq_t)sum;
    }
    else
    {
        /* Generic arbitrary‑precision implementation */
        seq_t t = 1;
        seq_t sum = 0;

        /* t initially equals (n-2)! */
        for (idx_t k = 1; k <= n - 2; ++k)
            t = t * (seq_t)k;

        for (idx_t m = 1; m <= max_m; ++m)
        {
            sum += t;                 /* add current term */

            if (m == max_m)
                break;                /* last term, no update needed */

            /* Update t to next term using recurrence:
               t_{m+1} = t_m * (2m)*(2m+1)*m /
                          ((n-2m-1)*(n-2m)*(m+1)) */
            t = t * (seq_t)(2 * m);
            t = t * (seq_t)(2 * m + 1);
            t = t * (seq_t)m;
            t = t / (seq_t)(n - 2 * m - 1);
            t = t / (seq_t)(n - 2 * m);
            t = t / (seq_t)(m + 1);
        }

        return sum;
    }
}
