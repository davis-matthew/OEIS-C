/*
 * A342488  a(n) = Sum_{d|n} phi(d)^(n/d+1).
 * https://oeis.org/A342488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d)^(n/d+1)
 * Range:     u8 n <= 14, u16 n <= 34, u32 n <= 74, u64 n <= 154,
 *            u128 n <= 314, big any n
 * Validated: standard, pass, 65 terms through n = 8141 (2026-11-01)
 * Helpers:   ipow, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/totient.h"

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t phid  = totient(d);
            seq_t expd  = (seq_t)(n / d) + 1;
            total += ipow(phid, expd);

            idx_t d2 = n / d;
            if (d2 != d) {
                seq_t phid2 = totient(d2);
                seq_t expd2 = (seq_t)(n / d2) + 1;
                total += ipow(phid2, expd2);
            }
        }
    }
    return total;
}
