/*
 * A351269  Sum of the 6th powers of the squarefree divisors of n.
 * https://oeis.org/A351269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p|n} (1 + p⁶)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1621,
 *            u128 n <= 10000, big any n
 * Helpers:   cube_power_six
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of the 6th powers of the squarefree divisors of n. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/cube_power_six.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* Handle factor 2 separately to allow i+=2 optimization. */
    if ((temp & 1) == 0) {
        seq_t term = (seq_t)1 + cube_power_six((idx_t)2);
        result *= term;
        while ((temp & 1) == 0)
            temp >>= 1;  /* temp /= 2; safe for uint64_t */
    }

    /* Trial divide by odd numbers up to √temp. */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            seq_t term = (seq_t)1 + cube_power_six(p);
            result *= term;
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* If anything remains, it's a prime factor > √original n. */
    if (temp > 1) {
        seq_t term = (seq_t)1 + cube_power_six(temp);
        result *= term;
    }

    return result;
}
