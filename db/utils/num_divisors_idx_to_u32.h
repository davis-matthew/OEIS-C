/* num_divisors_idx_to_u32: the number of divisors of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIVISORS_IDX_TO_U32_H
#define OEISC_UTILS_NUM_DIVISORS_IDX_TO_U32_H
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Compute τ(n) for n > MAX_TAU_LIMIT by trial division   */
static uint32_t num_divisors_idx_to_u32(idx_t n)
{
    idx_t m = n;
    uint32_t tau = 1;
    idx_t p = 2;
    while (p * p <= m)
    {
        if (m % p == 0)
        {
            uint32_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
        ++p;
    }
    if (m > 1)               /* remaining prime factor */
        tau *= 2;
    return tau;
}

#endif
