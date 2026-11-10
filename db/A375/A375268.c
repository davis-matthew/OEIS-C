/*
 * A375268  Row sums of A375266.
 * https://oeis.org/A375268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of the iterates of the map x -> (x/3 if 3|x, x/2 if
 *            2|x, else 3x+1) from n down to 1, including n and 1 (row sums of
 *            A375266)
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9734 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t s = (seq_t)1;
    while (x != (seq_t)1) {
        s += x;
        if (x % (seq_t)3 == (seq_t)0) x = x / (seq_t)3;
        else if (x % (seq_t)2 == (seq_t)0) x = x / (seq_t)2;
        else x = x * (seq_t)3 + (seq_t)1;
    }
    return s;
}
