/*
 * A347175  Sum of 4th powers of odd divisors of n that are <= sqrt(n).
 * https://oeis.org/A347175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d|n, d odd, d <= floor(sqrt(n))} d^4 (OEIS A347175)
 * Range:     u8 n <= 24, u16 n <= 288, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple integer square‑root routine
   and brute‑force divisor enumeration.  It works for any seq_t
   and any idx_t that fits in an unsigned 64‑bit integer. */

#include <stdint.h>

/* integer square root: floor(sqrt(n)) for 0 <= n <= UINT64_MAX */
static inline uint64_t isqrt64(uint64_t n)
{
    uint64_t x = 0, bit = (uint64_t)1 << 62;          /* highest power of four <= UINT64_MAX */
    while (bit > 0)
    {
        uint64_t next = x + bit;
        if (next <= n)
        {
            n -= next;
            x = (x >> 1) + bit;
        }
        else
        {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Use 64‑bit arithmetic for intermediate sums */
    uint64_t sum = 0;
    uint64_t N   = (uint64_t)n;
    uint64_t sqrt_n = isqrt64(N);

    /* Enumerate odd divisors d <= sqrt_n of N */
    for (uint64_t d = 1; d <= sqrt_n; d += 2)
    {
        if (N % d == 0)
        {
            uint64_t v = d * d;   /* d^2 */
            v = v * v;            /* d^4 */
            sum += v;
        }
    }

    /* Convert to seq_t (risk of overflow is allowed if value doesn't fit) */
    return (seq_t)sum;
}
