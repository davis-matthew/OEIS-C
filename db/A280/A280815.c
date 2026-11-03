/*
 * A280815  a(n) = A049502(cototient(n)).
 * https://oeis.org/A280815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A049502(A051953(n))
 * Range:     u8 n <= 1274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 125 terms through n = 9754 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = major index (2nd definition) of cototient(n) */

/* external function required by the framework */
#include <stdint.h>

static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

/* computes the "major index" of an integer m.
   For the binary expansion of m, read bits from least
   significant to most significant.  Sum the indices
   (starting from 1 at the least significant bit) of
   those positions i where bit i > bit i+1.  This
   matches OEIS A049502. */
static seq_t major_index_binary(seq_t m)
{
    seq_t sum = 0;
    /* loop over bit positions i where i+1 is still a valid bit */
    for (idx_t i = 0; (m >> (i + 1)) != 0; ++i)
    {
        if (((m >> i) & 1) && !((m >> (i + 1)) & 1))
            sum += (seq_t)(i + 1);
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* cototient(n) = n - phi(n) */
    seq_t phi_n = phi(n);           /* Euler's totient of n */
    seq_t cot   = (seq_t)n - phi_n; /* n - phi(n) */

    /* major index (2nd definition) of cototient(n) */
    return major_index_binary(cot);
}
