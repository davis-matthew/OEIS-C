/*
 * A266606  Binary representation of the n-th iteration of the "Rule 39"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(n/2) if n is even; a(n) =
 *            ((10^(3*floor(n/2)+2)-1)/9)*10^(floor(n/2)+1) +
 *            ((10^(floor(n/2))-1)/9) if n is odd
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    if ((n & 1) == 0) {                     // even n
        idx_t k = n >> 1;                   // k = n/2
        return pow10(k);
    } else {                                 // odd n
        idx_t k = n >> 1;                   // floor(n/2)
        idx_t L = 3 * k + 2;                // length of leading ones

        seq_t high = pow10(L);              // 10^L
        high = high - 1;                    // 10^L - 1
        high = high / 9;                    // (10^L - 1)/9  -> leading ones

        seq_t base = pow10(k + 1);          // 10^(k+1)

        seq_t value = high * base;          // leading ones shifted

        seq_t trailing = 0;
        if (k) {                            // trailing ones if k>0
            seq_t tenk = pow10(k);          // 10^k
            trailing = (tenk - 1) / 9;       // (10^k - 1)/9
        }

        value = value + trailing;           // final value
        return value;
    }
}
