/*
 * A375378  Value of the power tower formed by the numbers in the n-th
 *          composition (in standard order).
 * https://oeis.org/A375378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = value of the power tower formed by the numbers in the
 *            n-th composition (in standard order). The composition with index
 *            m is ordered by decreasing first part and recursively by the
 *            same rule for the remaining parts. The function computes the
 *            n-th tower value (a(0)=1)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   _compose_sum, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/_compose_sum.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;       /* a(0) is defined as 1 */

    /* find S such that 2^S - 1 >= n  (S is the sum of the composition) */
    unsigned int S = 64 - __builtin_clzll(n);   /* highest set bit position */

    /* offset within the compositions of sum S (0‑based) */
    idx_t offset = n - ((idx_t)1ULL << (S - 1));

    /* build the composition */
    seq_t parts[64];   /* maximum possible length (sum <= 64) */
    idx_t len = 0;
    _compose_sum((idx_t)S, offset, parts, &len);

    /* evaluate the power tower right‑to‑left: x1^(x2^(...)) */
    seq_t result = parts[len - 1];
    for (int i = (int)len - 2; i >= 0; --i)
        result = big_pow((seq_t)parts[i], result);

    return result;
}
