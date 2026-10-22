/*
 * A385212  a(n) = n^(mu(n)^2), where mu is the Möbius function (A008683).
 * https://oeis.org/A385212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is squarefree, otherwise 1 (i.e. a(n)=n^(mu(n)^2))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 1)
        return (seq_t)1;

    /* Check for square factors */
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            m /= p;                /* remove one copy of p */
            if (m % p == 0)        /* found p^2 | n */
                return (seq_t)1;   /* not square‑free */
            while (m % p == 0)     /* remove remaining copies (if any) */
                m /= p;
        }
    }
    return (seq_t)n; /* square‑free → a(n)=n */
}
