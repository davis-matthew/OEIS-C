/*
 * A322483  The number of semi-unitary divisors of n.
 * https://oeis.org/A322483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a is multiplicative with a(p^e) = floor((e+3)/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the number of semi‑unitary divisors of n.
   The function works for any idx_t and seq_t defined in the build. */
seq_t a(idx_t n)
{
    if (n == 0)            /* unused; safety */
        return 0;

    uint64_t m = (uint64_t)n;    /* work on 64‑bit value */
    seq_t   result = 1;          /* multiplicative accumulator */

    /* factor out 2 */
    uint64_t exp = 0;
    while ((m & 1ULL) == 0) {
        exp++;
        m >>= 1;
    }
    if (exp > 0)
        result *= (seq_t)((exp + 3) / 2);

    /* trial division for odd primes */
    for (uint64_t d = 3; d * d <= m; d += 2) {
        exp = 0;
        while (m % d == 0) {
            exp++;
            m /= d;
        }
        if (exp > 0)
            result *= (seq_t)((exp + 3) / 2);
    }

    /* remaining prime factor (if any) */
    if (m > 1)
        result *= (seq_t)((1 + 3) / 2);

    return result;
}
