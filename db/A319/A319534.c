/*
 * A319534  Number of integers x such that 1 <= x <= n and gcd(x,n) =
 *          gcd(x+2,n) = gcd(x+6,n) = 1.
 * https://oeis.org/A319534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A319534: Number of integers x such that
 * 1 ≤ x ≤ n and gcd(x,n) = gcd(x+2,n) = gcd(x+6,n) = 1.
 * The function is multiplicative. For each prime power p^e dividing n,
 *      a(p^e) = p^(e-1)                if p = 2 or p = 3
 *      a(p^e) = (p-3) * p^(e-1)        if p > 3
 * Thus a(n) = ∏ a(p^e) over all prime powers in n.
 *
 * The implementation factors n by trial division (sufficient for n ≤ 10000
 * and fast for larger values as well). All arithmetic is performed using
 * the type seq_t, which may be any unsigned integer type or an
 * arbitrary‑precision integer class.  The function is safe for any
 * n ≥ 1 and makes no assumptions about the order of calls.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t m = (uint64_t)n;         /* mutable copy of n */
    seq_t result = (seq_t)1;

    /* factorization by trial division */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p != 0) continue;

        uint64_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        /* compute p^(exp-1) as seq_t */
        seq_t p_pow = (seq_t)1;
        for (uint64_t i = 0; i < exp - 1; ++i)
            p_pow *= (seq_t)p;

        if (p == 2 || p == 3) {
            result *= p_pow;          /* p^(e-1) */
        } else {
            result *= (seq_t)(p - 3) * p_pow;  /* (p-3)*p^(e-1) */
        }
    }

    /* any remaining prime factor > sqrt(original n) */
    if (m > 1) {
        uint64_t p = m;
        if (p == 2 || p == 3) {
            /* p^(1-1) == 1, so do nothing */
        } else {
            result *= (seq_t)(p - 3); /* exp = 1 → factor = p-3 */
        }
    }

    return result;
}
