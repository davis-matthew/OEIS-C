/*
 * A381674  a(n) = product of numbers k < n such that 1 < gcd(k,n) and rad(k)
 *          != rad(n).
 * https://oeis.org/A381674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product{k < n : 1 < gcd(k,n) and rad(k) != rad(n)}
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 29, u128 n <= 53,
 *            big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

static seq_t rad_of_uint64(uint64_t m)
{
    if (m < 2)
        return 1;
    /* Factorization into primes */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(m, pr, ex, &cnt);
    seq_t rad = 1;
    for (size_t i = 0; i < cnt; ++i)
        rad *= (seq_t)pr[i];
    return rad;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 1;                     /* safety for non‑positive indices */

    /* For n = 1 the product over an empty set is 1 */
    if (n <= 2)
        return 1;

    /* Compute rad(n) once */
    seq_t rad_n = rad_of_uint64((uint64_t)n);

    seq_t prod = 1;
    for (idx_t k = 2; k < n; ++k)
    {
        /* Skip coprime k */
        if (gcd((seq_t)k, (seq_t)n) <= 1)
            continue;

        /* Compute rad(k) and skip if equal to rad(n) */
        if (rad_of_uint64((uint64_t)k) == rad_n)
            continue;

        prod *= (seq_t)k;
    }
    return prod;
}
