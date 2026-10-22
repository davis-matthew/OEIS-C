/*
 * A355263  a(n) = largest-nth-power(n, 3) * radical(n) = A053150(n) *
 *          A007947(n), where the largest-nth-power(n, e) is the largest
 *          positive integer b such that b^e divides n.
 * https://oeis.org/A355263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} p^(1 + ⌊e/3⌋) (largest-nth-power(n,3) *
 *            radical(n))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow_seq_u64(seq_t base, unsigned exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)n;          /* a(1)=1, a(0) not used */

    seq_t result = 1;
    idx_t remaining = n;
    idx_t p = 2;

    while (p <= remaining / p)
    {
        if (remaining % p == 0)
        {
            unsigned cnt = 0;
            while (remaining % p == 0)
            {
                remaining /= p;
                cnt++;
            }
            unsigned power = 1 + cnt / 3;          /* floor(e/3)+1 */
            result *= pow_seq_u64((seq_t)p, power);
        }
        p += (p == 2 ? 1 : 2);             /* 2, then odd numbers */
    }

    if (remaining > 1)                       /* remaining is prime > sqrt(n) */
    {
        result *= (seq_t)remaining;          /* power = 1 + 0 = 1 */
    }

    return result;
}
