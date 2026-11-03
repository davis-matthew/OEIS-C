/*
 * A345098  a(n) = Sum_{k=1..n} floor(n/k)^floor(n/k).
 * https://oeis.org/A345098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k)^floor(n/k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 363 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t k = 1;
    while (k <= n) {
        /* current value of floor(n/k) */
        idx_t t = n / k;
        /* next k where the value of floor(n/k) changes */
        idx_t next = n / t + 1;

        /* compute t^t */
        seq_t power = ipow((seq_t)t, (seq_t)t);

        /* add contribution of all k in [k, next) */
        seq_t count = (seq_t)(next - k);
        total += power * count;

        k = next;
    }
    return total;
}
