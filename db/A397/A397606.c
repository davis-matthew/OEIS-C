/*
 * A397606  a(n) = denominator of A069835(n)/2^n.
 * https://oeis.org/A397606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(2*n) = 2^(2*n - popcount(n)); a(2*n+1) = 2^(2*n - popcount(n)
 *            - 1) * (with a(0) = a(1) = 1). This implements OEIS A397606. *
 *            The function uses the helper pow2(exp) to construct powers of
 *            two in the * arbitrary‑precision build. The built‑in popcount
 *            works on 64‑bit values * and is sufficient bec...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 100,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Special cases for the offset */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Even indices */
    if ((n & 1u) == 0) {
        idx_t k  = n >> 1;                               /* n = 2*k   */
        idx_t pc = __builtin_popcountll((unsigned long long)k);
        idx_t exp = (k << 1) - pc;                       /* 2*k - popcount(k) */
        return pow2(exp);
    }

    /* Odd indices (n = 2*k + 1, k >= 1) */
    idx_t k  = (n - 1) >> 1;                               /* k = (n-1)/2 */
    idx_t pc = __builtin_popcountll((unsigned long long)k);
    idx_t exp = (k << 1) - pc - 1;                       /* 2*k - popcount(k) - 1 */
    return pow2(exp);
}
