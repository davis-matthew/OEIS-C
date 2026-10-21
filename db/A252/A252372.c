/*
 * A252372  Characteristic function for A251726: a(n) = 1 if n > 1 and gpf(n)
 *          < spf(n)^2, otherwise 0; here spf(n) and gpf(n) (smallest and
 *          greatest prime factor of n) are sequences A020639(n) and
 *          A006530(n).
 * https://oeis.org/A252372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n>1 and gpf(n) < spf(n)^2, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gpf.h"

/* Smallest prime factor of n (returns n if n is prime or 1) */
static uint64_t spf_uint64(uint64_t n)
{
    if (n < 2)
        return n;
    for (uint64_t i = 2; i * i <= n; ++i)
        if (n % i == 0)
            return i;
    return n; /* n is prime */
}

seq_t a(idx_t n)
{
    if (n <= 1U)
        return (seq_t)0; /* a(1) = 0 by definition */

    uint64_t nn = (uint64_t)n;
    uint64_t sp = spf_uint64(nn);
    uint64_t gp = gpf(nn);

    /* Compare using native integer to avoid overflow of seq_t */
    if (gp < sp * sp)
        return (seq_t)1;
    else
        return (seq_t)0;
}
