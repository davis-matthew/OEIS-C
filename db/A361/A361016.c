/*
 * A361016  a(n) = 1 if A004718(n) = 0, otherwise 0, where A004718 is the
 *          Danish composer Per Nørgård's "infinity sequence".
 * https://oeis.org/A361016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A343029(n) = A343030(n)] (Iverson bracket); equivalently
 *            a(n) = 1 if A004718(n) = 0, else 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9824 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t even_cnt = 0, odd_cnt = 0, zeros = 0;
    idx_t t = n;
    while (t) {
        if (t & 1ULL) {
            if (zeros & 1ULL) odd_cnt++;
            else even_cnt++;
        } else {
            zeros++;
        }
        t >>= 1;
    }
    return (seq_t)(even_cnt == odd_cnt ? 1 : 0);
}
