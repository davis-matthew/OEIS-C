/*
 * A285958  Positions of 0 in A285957; complement of A285959.
 * https://oeis.org/A285958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285957, the Thue-Morse word
 *            with every 1 that follows a 0 deleted; computed by a linear scan
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9195 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t k = 0;
    idx_t pos = 0;
    idx_t zeros = 0;
    int prev = 1;
    while (1) {
        int bit = __builtin_popcountll((unsigned long long)k) & 1;
        if (bit == 1 && prev == 0) {
            prev = bit;
            k++;
            continue;
        }
        pos++;
        if (bit == 0) {
            zeros++;
            if (zeros == n) return (seq_t)pos;
        }
        prev = bit;
        k++;
    }
}
