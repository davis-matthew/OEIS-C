/*
 * A348476  Number of compositions of n into exactly n nonnegative parts such
 *          that all positive parts are odd.
 * https://oeis.org/A348476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n, k≡n (mod 2)} C(n,k) * C((n+k)/2 - 1, k-1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 78,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Starting k has the same parity as n */
    idx_t k      = (n & 1) ? 1 : 2;
    seq_t result = 0;

    /* Number of ways to choose the positions of the non‑zero parts */
    seq_t pos = binomial(n, k);                     /* C(n,k) */

    /* Parameters of the composition of n into k odd parts */
    idx_t m = (n + k) >> 1; /* (n + k)/2   */
    m -= 1;                 /* (n + k)/2 - 1 */
    idx_t r = k - 1;        /* number of parts minus 1 */
    seq_t parts = binomial(m, r);                /* C(m,r) */

    while (1) {
        result += pos * parts;                    /* add current contribution */

        /* Stop if next k would exceed n */
        if (k + 2 > n)
            break;

        /* Update pos: C(n, k+2) = C(n,k) * (n-k)(n-k-1) / ((k+1)(k+2)) */
        pos = (pos * (seq_t)(n - k) * (seq_t)(n - k - 1))
                        / ((seq_t)(k + 1) * (seq_t)(k + 2));

        /* Update parts: C(m+1, r+2) = C(m,r) * (m+1)(m-r) / ((r+2)(r+1)) */
        seq_t num = parts * (seq_t)(m + 1) * (seq_t)(m - r);
        seq_t den = (seq_t)(r + 2) * (seq_t)(r + 1);
        parts = num / den;

        /* Advance indices for the next iteration */
        k += 2;
        m += 1;
        r += 2;
    }

    return result;
}
