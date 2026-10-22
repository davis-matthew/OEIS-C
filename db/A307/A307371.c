/*
 * A307371  Numbers k such that the digits of sqrt(k) begin with k.
 * https://oeis.org/A307371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A307371: numbers k such that the digits of sqrt(k) begin with k.
   a(1) = 0, a(2) = 1.
   For n >= 3:
       let m = (n-3)/3 + 1          // position of the block of 3 terms
       let pos = (n-3) % 3          // 0 -> 10^(2m)-2, 1 -> 10^(2m)-1, 2 -> 10^(2m)
       a(n) = 10^(2m) - (2 - pos)
   Uses the theorem that every term (except 0 and 1) is 10^(2m), 10^(2m)-1,
   or 10^(2m)-2 for some m >= 1. */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 1;
    idx_t k   = n - 3;           // 0‑based offset within blocks of 3
    idx_t m   = k / 3 + 1;        // block number, m >= 1
    idx_t pos = k % 3;            // 0,1,2
    seq_t val = pow10(2 * m);     // 10^(2m)

    if (pos == 2) return val;          // 10^(2m)
    if (pos == 1) return val - 1;      // 10^(2m)-1
    return val - 2;                    // 10^(2m)-2
}
