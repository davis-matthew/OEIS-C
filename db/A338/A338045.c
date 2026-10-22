/*
 * A338045  G.f.: Sum_{k>=0} x^(2^k) / (1 - x^(2^k))^3.
 * https://oeis.org/A338045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n % 2 == 0 ? a(n / 2) : 0) + n * (n + 1) / 2
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1 (offset 1).  For safety, if n == 0
       return 0; no partial computation is performed. */
    if (n == 0)
        return 0;

    seq_t result = 0;
    seq_t cur = (seq_t)n;

    /* Recurrence derived from the generating‑function equation
       A(x) = A(x²) + x/(1 - x)³.  The binomial coefficient
       (n+1 choose 2) expands to n * (n+1) / 2. */
    while (1) {
        seq_t term = cur * (cur + 1) / 2;
        result += term;
        /* If n is odd, no further term from the A(x²) part. */
        if (cur & 1)
            break;
        cur >>= 1;            /* n /= 2 for next iteration */
    }
    return result;
}
