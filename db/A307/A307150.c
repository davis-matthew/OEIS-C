/*
 * A307150  Row 6 of array in A059897.
 * https://oeis.org/A307150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) swaps the exponents of 2 and 3 in n: if e₂ is even add 1
 *            else subtract 1; if e₃ is even add 1 else subtract 1. This
 *            yields the self‑inverse permutation of A307150
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    idx_t tmp = n;
    idx_t e2 = 0, e3 = 0;

    /* count powers of 2 */
    while ((tmp & 1) == 0) {
        e2++;
        tmp >>= 1;
    }

    /* count powers of 3 */
    while (tmp % 3 == 0) {
        e3++;
        tmp /= 3;
    }

    seq_t rest = (seq_t)tmp;            /* remaining odd part (no factor 2 or 3) */
    idx_t e2p = (e2 & 1) ? e2 - 1 : e2 + 1; /* new exponent of 2 */
    idx_t e3p = (e3 & 1) ? e3 - 1 : e3 + 1; /* new exponent of 3 */

    seq_t res = rest * pow2(e2p);
    res *= pow3(e3p);

    return res;
}
