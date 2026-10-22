/*
 * A305233  Smallest k such that binomial(k, floor(k/2)) >= n.
 * https://oeis.org/A305233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 5553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest k such that binomial(k, floor(k/2)) >= n. */
#include <stdint.h>
#include <stddef.h>

/* Helper: compute the central binomial coefficient C(k, floor(k/2)). */
static seq_t central_binomial(idx_t k)
{
    idx_t half = k >> 1;           /* floor(k/2) */
    seq_t result = (seq_t)1;       /* start at 1 */

    /* multiplicative formula: C(k, half) = Π_{i=1}^{half} (k - half + i) / i */
    for (idx_t i = 1; i <= half; ++i)
    {
        result = result * ((seq_t)(k - half + i));
        result = result / (seq_t)i;
    }
    return result;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    const seq_t sn = (seq_t)n;     /* n as seq_t for comparison */

    for (idx_t k = 1; ; ++k)
    {
        if (central_binomial(k) >= sn)
            return (seq_t)k;
    }
}
