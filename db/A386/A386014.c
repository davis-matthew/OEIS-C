/*
 * A386014  Distinct values occurring in first differences of A030655(n),
 *          listed in order of first appearance.
 * https://oeis.org/A386014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 2*(10^k + 1) for odd n, k = (n+1)/2 a(n) = 9*100^k -
 *            7*10^k + 2 for even n, k = n/2 where 10^k and 100^k are computed
 *            with pow10()
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 23,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* Offset is 1; or guard against invalid input */
        return 0;

    if (n & 1) {                /* odd index */
        idx_t k = (n + 1) >> 1;
        seq_t tenk = pow10(k); /* 10^k */
        return 2 * (tenk + 1);
    } else {                    /* even index */
        idx_t k = n >> 1;
        seq_t tenk = pow10(k);        /* 10^k */
        seq_t pow10_2k = tenk * tenk; /* 10^(2k) = 100^k */
        return 9 * pow10_2k - 7 * tenk + 2;
    }
}
