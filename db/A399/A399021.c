/*
 * A399021  a(n) is the minimum absolute difference between the legs of a
 *          primitive Pythagorean triangle with larger Euclid parameter n.
 * https://oeis.org/A399021
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ |n^2 - k^2 - 2*n*k| : 1 ≤ k < n, gcd(n,k)=1, (n-k)
 *            odd }
 * Range:     u8 n <= 23, u16 n <= 4277, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for n<2, but safe */

    seq_t best = 0;
    bool ok = false;              /* whether we found a candidate */

    for (idx_t k = 1; k < n; ++k) {
        /* parity condition: n-k must be odd */
        if (((n - k) & (idx_t)1) == 0) continue;

        /* coprime condition */
        if (gcd((seq_t)n, (seq_t)k) != 1) continue;

        /* compute |(n-k)^2 - 2*k^2| safely in 64‑bit signed arithmetic */
        long long x = (long long)n - (long long)k;      /* n-k */
        long long kk = (long long)k;
        long long diff = x * x - 2LL * kk * kk;
        long long absDiff = diff >= 0 ? diff : -diff;

        if (!ok || absDiff < (long long)best) {
            best = (seq_t)absDiff;
            ok = true;
            if (best == 1) break;     /* minimal possible value */
        }
    }

    /* If no admissible k exists (should not happen for n≥2),
       return 0 as a safe fallback. */
    return ok ? best : 0;
}
