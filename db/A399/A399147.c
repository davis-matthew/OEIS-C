/*
 * A399147  Number of distinct proper side-crossing points among regular
 *          polygons inscribed in a common circle and aligned by their side
 *          midpoints.
 * https://oeis.org/A399147
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*Σ_{3 ≤ m < k ≤ n} (m – [val_2(m)=val_2(k)]*gcd(m,k))
 *            where val_2(r) is the exponent of 2 in r's prime factorisation
 * Range:     u8 n <= 9, u16 n <= 58, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, val_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/val_2.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* pre‑compute val_2 for all numbers up to n */
    idx_t *val = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!val)
        abort();
    for (idx_t i = 3; i <= n; ++i)
        val[i] = val_2(i);

    /* sum over all pairs (m,k) with same val_2 */
    seq_t sum_gcd = 0;
    for (idx_t m = 3; m < n; ++m)
    {
        idx_t vm = val[m];
        for (idx_t k = m + 1; k <= n; ++k)
        {
            if (vm == val[k])
                sum_gcd += gcd((seq_t)m, (seq_t)k);
        }
    }

    free(val);

    /* compute Σ_{m=3}^{n} m*(n-m) in a compact form */
    seq_t nn = (seq_t)n;
    seq_t sum_m  = nn * (nn + 1) / 2 - (seq_t)3;                /* Σ m  */
    seq_t sum_m2 = nn * (nn + 1) * (2 * nn + 1) / 6 - (seq_t)5; /* Σ m² */
    seq_t sum_m_val = nn * sum_m - sum_m2;                       /* Σ m*(n-m) */

    /* a(n) = 2 * (sum_m_val – sum_gcd) */
    return 2 * (sum_m_val - sum_gcd);
}
