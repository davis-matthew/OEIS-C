/*
 * A260233  Smallest prime factor of the n-th hexagonal number (A000384).
 * https://oeis.org/A260233
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A020639(A000384(n)) – smallest prime factor of the n‑th
 *            hexagonal number H_n = n*(2n-1). The function uses trial
 *            division with odd divisors. It works for any seq_t that supports
 *            the usual arithmetic and comparison operators and for any n ≥ 2
 * Range:     u8 n <= 12, u16 n <= 240, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* undefined for n < 2, but safe fallback */

    /* Compute the nth hexagonal number: H_n = n*(2n-1)  */
    seq_t hex = (seq_t)n * (((seq_t)n) * 2 - 1);

    /* The smallest prime factor is always 2 if hex is even                 */
    if (hex % 2 == 0)
        return 2;

    /* Trial division with odd candidates up to sqrt(hex)                   */
    seq_t i = 3;
    while (i * i <= hex) {
        if (hex % i == 0)
            return i;
        i += 2;
    }

    /* If no divisor found, hex itself is prime (and >2)                     */
    return hex;
}
