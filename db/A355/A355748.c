/*
 * A355748  Number of ways to choose a sequence of divisors, one of each part
 *          of the n-th composition in standard order.
 * https://oeis.org/A355748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of divisor counts of the parts of the n‑th
 *            composition in standard order
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                  /* empty composition → product 1 */

    idx_t k = n;
    uint8_t pos[64];                /* positions of 1‑bits (1‑indexed) */
    size_t m = 0;
    idx_t bit = 1;                  /* current bit position (1‑based) */

    /* collect set‑bit positions */
    while (k)
    {
        if (k & 1)
            pos[m++] = bit;
        k >>= 1;
        bit++;
    }

    seq_t prod = 1;
    /* iterate parts in reverse order of the differences between positions */
    for (size_t i = m; i > 0; --i)
    {
        seq_t part;
        if (i == 1)
            part = (seq_t)pos[0];                     /* first difference */
        else
            part = (seq_t)(pos[i-1] - pos[i-2]);       /* subsequent differences */

        /* compute τ(part), the number of divisors of part */
        seq_t tau = 0;
        seq_t d = 1;
        while (d * d <= part)
        {
            if (part % d == 0)
            {
                if (d * d == part)
                    tau += 1;     /* perfect square */
                else
                    tau += 2;     /* pair of divisors */
            }
            d++;
        }

        prod *= tau;
    }

    return prod;
}
