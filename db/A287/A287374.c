/*
 * A287374  Positions of 1 in A101666.
 * https://oeis.org/A287374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2n - 1 if A101666(n-1) != 0, else 2n (each pair of terms
 *            A101666(2k),A101666(2k+1) holds exactly one 1)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9295 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static int A287374_child(int parent, bool bit)
{
    switch (parent) {
        case 0: return bit ? 1 : 0;
        case 1: return bit ? 2 : 1;
        default: return bit ? 0 : 1;
    }
}

static int A287374_A101666(idx_t n)
{
    int val = 0;
    if (n == 0) return 0;
    int top = 63;
    while (top > 0 && ((n >> top) & 1) == 0) top--;
    for (int i = top; i >= 0; i--) {
        val = A287374_child(val, ((n >> i) & 1) != 0);
    }
    return val;
}

seq_t a(idx_t n)
{
    int s = A287374_A101666(n - 1);
    seq_t two_n = (seq_t)n * 2;
    if (s != 0) return two_n - 1;
    return two_n;
}
