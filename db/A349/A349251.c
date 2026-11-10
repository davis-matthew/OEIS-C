/*
 * A349251  a(n) is the integer reached after repeated application of the map
 *          x->A349194(x) or -1 if this process does not terminate.
 * https://oeis.org/A349251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = x reached by iterating x -> A349194(x) from n until a
 *            fixed point (A349194(x) = x); -1 if it never terminates (cycle
 *            or divergence). A349194(x) = Prod_{i=1..len(x)} (sum of first i
 *            digits of x)
 * Range:     u8 n <= 265, u16 n <= 378, u32 n <= 378, u64 n <= 378,
 *            u128 n <= 378, big any n
 * Validated: standard, pass, 106 terms through n = 370 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A349194_a(seq_t n, int *toolong)
{
    unsigned char digits[80];
    int d = 0;
    seq_t tmp = n;
    *toolong = 0;
    while (tmp > 0) {
        if (d >= 70) { *toolong = 1; return (seq_t)0; }
        digits[d++] = (unsigned char)(long long)(tmp % 10);
        tmp /= 10;
    }
    seq_t sum = (seq_t)0;
    seq_t prod = (seq_t)1;
    for (int i = d - 1; i >= 0; --i) {
        sum = sum + (seq_t)(int)digits[i];
        prod = prod * sum;
    }
    return prod;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    int toolong = 0;
    for (int step = 0; step < 3000; ++step) {
        seq_t y = A349194_a(x, &toolong);
        if (toolong) break;
        if (y == x) return x;
        x = y;
    }
    return (seq_t)0 - (seq_t)1;
}
