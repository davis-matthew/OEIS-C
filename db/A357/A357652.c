/*
 * A357652  Number of pairs of Dyck paths of semilength n such that the
 *          midpoint of the first is not below the midpoint of the second.
 * https://oeis.org/A357652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A000108(n)^2 + A129123(n))/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A129123(n) = sum_{h} f(n,h)^4  where f(n,h) = (h+1)/(n+1) * binomial(n+1,(n-h)/2) */

#include <stddef.h>
#include "../utils/catalan.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Catalan number C(n) */
    seq_t cat = catalan(n);
    seq_t cat_sq = cat * cat;

    seq_t E = 0;                /* sum over h of f(n,h)^4 */

    /* Determine starting height (same parity as n) */
    idx_t start = (n & 1) ? 1 : 0;
    idx_t k = (n - start) / 2;            /* k = (n - h)/2 */

    /* First binomial value */
    seq_t bin = binomial(n + 1, k);

    for (idx_t h = start; h <= n; h += 2, k--)
    {
        /* f(n,h) = (h+1)/(n+1) * binomial(n+1,k) */
        seq_t f = (bin * (seq_t)(h + 1)) / (seq_t)(n + 1);

        /* f^4 */
        seq_t f2 = f * f;
        seq_t f4 = f2 * f2;
        E += f4;

        /* Update binomial for next h if needed */
        if (h + 2 <= n)
        {
            seq_t num = bin * (seq_t)k;               /* k is current value before decrement */
            seq_t denom = (seq_t)((n + 1) - k + 1);   /* (n+1)-k+1 = n-k+2 */
            bin = num / denom;
        }
    }

    /* Final result: (cat_sq + E)/2 */
    seq_t sum = cat_sq + E;
    return sum / 2;
}
