/* mobius: the Moebius function mu(n).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MOBIUS_H
#define OEISC_UTILS_MOBIUS_H
#include <stdint.h>

static seq_t mobius(idx_t m)
{
    if (m == 0) return 0;          /* defined as 0, but 0 is never passed */
    seq_t sign = 1;                /* product of (–1) for each prime factor */
    idx_t tmp = m;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int cnt = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++cnt;
                if (cnt > 1)
                    return 0;     /* not square‑free */
            }
            sign = -sign;
        }
    }
    if (tmp > 1)                   /* remaining prime factor */
        sign = -sign;
    return sign;
}

#endif
