/*
 * A270301  Numbers which are representable as a sum of sixteen but no fewer
 *          consecutive nonnegative integers.
 * https://oeis.org/A270301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1570, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers representable as a sum of sixteen but no fewer consecutive
   nonnegative integers: a(n) is the n‑th integer ≡8 (mod 16) that cannot be
   expressed as the sum of k consecutive integers for any 2 ≤ k < 16. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 8;                      /* initial k so that 8 + 16*k = 136 */
    while (1) {
        seq_t val = (seq_t)8 + (seq_t)16 * (seq_t)k;
        bool ok = true;

        /* test all shorter lengths 2 … 15 */
        for (int m = 2; m < 16; ++m) {
            seq_t offset = (seq_t)m * (seq_t)(m - 1) / 2;
            if (val < offset) continue;          /* a would be negative */
            seq_t diff = val - offset;
            if (diff % m == 0) {                 /* representable with m terms */
                ok = false;
                break;
            }
        }

        if (ok) {
            ++count;
            if (count == n) return val;
        }
        ++k;
    }
}
