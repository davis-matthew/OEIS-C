/*
 * A379970  a(n) = 1 if n is twice its squarefree kernel (A007949), otherwise
 *          0.
 * https://oeis.org/A379970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 2*A007949(n) (squarefree kernel), else 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 141 terms through n = 9714 (2026-11-01)
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) using the property that n must be 4 × product of distinct odd primes. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"

/* External linkage: seq_t is the return type, idx_t is the index type. */
seq_t a(idx_t n)
{
    /* If n is not divisible by 4, or has more than two factors of 2, it cannot satisfy the condition. */
    if ((n & 3) != 0)          /* n % 4 != 0 */
        return 0;
    if ((n & 7) == 0)          /* n % 8 == 0 (i.e., 2^3 or higher) */
        return 0;

    /* Now n = 4 * m where m is odd. Check that m is square‑free. */
    idx_t m = n >> 2;          /* m = n / 4 */

    return is_squarefree((seq_t)m) ? 1 : 0;
}
