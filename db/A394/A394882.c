/*
 * A394882  a(n) = 3*a(n-1) + (n mod 2)*floor(2^(n+1)/3) for n > 1, a(1) = 1.
 * https://oeis.org/A394882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula implemented: a(1) = 1; for n > 1, a(n) = 3*a(n-1) + (n mod 2)*floor(2^(n+1)/3) */
#include <stddef.h>                        /* size_t, idx_t */
#include <stdint.h>                       /* uint64_t */
#include "../utils/pow2.h"                 /* pow2(idx_t) → seq_t */

/* External sequence type */
extern seq_t a(idx_t n)
{
    if (n == 0)                /* below the OEIS offset – undefined, return 0 */
        return (seq_t)0;

    seq_t prev = 1;            /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term = 0;
        if (i & 1U)            /* odd index: add floor(2^(i+1)/3) */
            term = pow2(i + 1U) / (seq_t)3;
        prev = prev * 3 + term;
    }

    return prev;
}
