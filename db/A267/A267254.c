/*
 * A267254  Binary representation of the n-th iteration of the "Rule 111"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binary representation of the n‑th iteration of *
 *            elementary cellular automaton Rule 111 starting with a single ON
 *            cell. * Base cases: * a(0)=1, a(1)=110, * a(2)=111,
 *            a(3)=1111010, a(4)=11111, * For even n>4: a(n)=1011111 (decimal
 *            value of the binary string * "1011111"). * For odd n≥5: a(n)...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    switch (n) {
        case 0u: return 1;
        case 1u: return 110;
        case 2u: return 111;
        case 3u: return 1111010;
        case 4u: return 11111;
        default:
            if ((n & 1u) == 0u)          // even n > 4
                return 1011111;         // decimal value of "1011111"
            else {                       // odd n ≥ 5
                seq_t val = pow10(2 * n + 1);  // 10^(2n+1)
                val /= 9;                       // floor division
                val -= 11101;
                return val;
            }
    }
}
