/*
 * A302135  Primes of form 9*k^k - 8.
 * https://oeis.org/A302135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*(2*n+5)^(2*n+5) - 8 (OEIS A302135)
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 3 terms through n = 3 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset is 1; unused case */

    /* k = 2*n + 5  */
    seq_t k = (seq_t)(2*n + 5);

    /* Compute k^k */
    seq_t pow_val = ipow(k, k);

    /* Multiply by 9 and subtract 8 */
    seq_t result = pow_val * (seq_t)9;
    result -= (seq_t)8;

    return result;
}
