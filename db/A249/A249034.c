/*
 * A249034  Odd numbers missing from A171947.
 * https://oeis.org/A249034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*k+1 where k is the n-th positive integer with v_2(k)
 *            even (search/count)
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9798 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        k++;
        idx_t v = k;
        unsigned e = 0;
        while ((v & 1) == 0) {
            v >>= 1;
            e++;
        }
        if ((e & 1) == 0)
            count++;
    }
    return (seq_t)k * 4 + 1;
}
