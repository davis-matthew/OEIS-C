/*
 * A289660  a(n) = A037276(n) - n.
 * https://oeis.org/A289660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A037276(n) – n A037276(n) is the concatenation of the
 *            prime factors of n (with multiplicity), ordered increasingly.
 *            The function returns the concatenated number minus n
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digits_u64.h"

static seq_t pow10_u64(uint64_t e)
{
    seq_t res = 1;
    seq_t base = 10;
    while (e)
    {
        if (e & 1ULL)
            res *= base;
        base *= base;
        e >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) = 0 */
        return 0;

    idx_t temp = n;
    seq_t result = 0;

    /* factor out 2 */
    while ((temp % 2ULL) == 0)
    {
        int d = digits_u64(2ULL);
        seq_t ten = pow10_u64(d);
        result = result * ten + (seq_t)2ULL;
        temp /= 2ULL;
    }

    /* odd prime factors */
    for (idx_t p = 3; p <= temp / p; p += 2)
    {
        while ((temp % p) == 0)
        {
            int d = digits_u64(p);
            seq_t ten = pow10_u64(d);
            result = result * ten + (seq_t)p;
            temp /= p;
        }
    }

    /* remaining prime factor > sqrt(temp) */
    if (temp > 1ULL)
    {
        int d = digits_u64(temp);
        seq_t ten = pow10_u64(d);
        result = result * ten + (seq_t)temp;
    }

    result -= (seq_t)n;   /* a(n) = A037276(n) – n */
    return result;
}
