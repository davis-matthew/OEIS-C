/* is_squarefree_u64: whether n is squarefree.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IS_SQUAREFREE_U64_H
#define OEISC_UTILS_IS_SQUAREFREE_U64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- test square‑free using trial division ---------- */
static bool is_squarefree_u64(uint64_t n)
{
    if (n <= 1) return true;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            int cnt = 0;
            while (n % p == 0)
            {
                n /= p;
                ++cnt;
                if (cnt > 1)           /* found p^2 | n */
                    return false;
            }
        }
    }
    return true;
}

#endif
