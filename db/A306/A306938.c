/*
 * A306938  If n is divisible by 3 then a(n) = n/3 otherwise a(n) =
 *          floor(n*sqrt(3)).
 * https://oeis.org/A306938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/3 if n mod 3 = 0, otherwise a(n) = floor(n*sqrt(3))
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Case n divisible by 3 */
    if (n % 3 == 0)
        return (seq_t)(n / 3);

    /* Compute floor(n*sqrt(3)) as floor(sqrt(3*n*n)) */
    /* Use 128‑bit arithmetic to avoid overflow for reasonable n */
    uint64_t nn = (uint64_t)n;
    unsigned __int128 prod = (unsigned __int128)nn * nn * 3;

    /* Approximate sqrt with long double, then correct */
    long double approx = sqrtl((long double)prod);
    uint64_t val = (uint64_t)approx;          /* truncation gives a lower bound */

    /* Adjust for any rounding error of sqrtl */
    while ((unsigned __int128)(val + 1) * (val + 1) <= prod)
        val++;
    while ((unsigned __int128)val * val > prod)
        val--;

    return (seq_t)val;
}
