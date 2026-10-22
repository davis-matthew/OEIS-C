/*
 * A342700  For any number n with binary expansion (b(1), b(2), ..., b(k)),
 *          the binary expansion of a(n) is (1-floor((b(k)+b(1)+b(2))/2),
 *          1-floor((b(1)+b(2)+b(3))/2), ..., 1-floor((b(k-1)+b(k)+b(1))/2)).
 * https://oeis.org/A342700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is produced by taking, for each triple of consecutive bits
 *            (with wrap‑around) of n, the value 1 if the sum of the triple is
 *            ≤ 1, otherwise 0. This yields the sequence A342700
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* special‑case n == 0 */
    if (n == 0)
        return 0;

    /* length of binary representation (≥1 for n>0) */
    unsigned k = 64 - __builtin_clzll((unsigned long long)n);

    seq_t res = 0;
    for (unsigned i = 0; i < k; ++i)
    {
        /* indices of the triple (wrap‑around) */
        unsigned prev = (i == 0) ? k - 1U : i - 1U;
        unsigned next = (i + 1 == k) ? 0U : i + 1U;

        /* extract the three bits */
        unsigned b_prev = (n >> (k - 1U - prev)) & 1U;
        unsigned b_curr = (n >> (k - 1U - i))    & 1U;
        unsigned b_next = (n >> (k - 1U - next)) & 1U;

        /* determine the output bit */
        unsigned outbit = ((b_prev + b_curr + b_next) <= 1U) ? 1U : 0U;

        /* append to result */
        res = (res << 1) | outbit;
    }
    return res;
}
