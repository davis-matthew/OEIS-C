/*
 * A333817  G.f.: Sum_{k>=1} x^(k*(5*k - 3)/2) / (1 - x^(5*k)).
 * https://oeis.org/A333817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of pairs (i,k) with i ≥ 0, k ≥ 1 such that n =
 *            k*(5*k-3)/2 + 5*k*i, i.e. n = H_k + 5k·i, where H_k is the k‑th
 *            heptagonal number
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;              /* result, stored in seq_t */
    for (idx_t k = 1; ; ++k)     /* iterate over index difference k */
    {
        idx_t hk = k * (5 * k - 3) / 2;   /* generate heptagonal number H_k */
        if (hk > n)               /* further H_k grow only larger */
            break;
        idx_t diff = n - hk;      /* remaining part must be a multiple of 5k */
        if (diff % (5 * k) == 0)
            ++count;
    }
    return count;
}
