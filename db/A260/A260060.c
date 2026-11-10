/*
 * A260060  Least number such that exactly n iterations of A034690 are
 *          required to reach one of the fixed points, 1 or 15.
 * https://oeis.org/A260060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k such that exactly n iterations of A034690
 *            starting at k reach 1 or 15; found by incremental search
 * Range:     u8 n <= 19, u16 n <= 20, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static uint64_t A260060_ds(uint64_t x)
{
    uint64_t s = 0;
    while (x) { s += x % 10; x /= 10; }
    return s;
}

static uint64_t A260060_f(uint64_t m)
{
    uint64_t sum = 0;
    for (uint64_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            sum += A260060_ds(i);
            uint64_t j = m / i;
            if (j != i) sum += A260060_ds(j);
        }
    }
    return sum;
}

static int A260060_steps(uint64_t k)
{
    int c = 0;
    uint64_t x = k;
    while (x != 1 && x != 15) {
        x = A260060_f(x);
        ++c;
        if (c > 200) return -1;
    }
    return c;
}

seq_t a(idx_t n)
{
    static uint64_t found[256];
    static bool have[256];
    static uint64_t next_k = 1;

    if (n >= 256) return 0;
    while (!have[n]) {
        int s = A260060_steps(next_k);
        if (s >= 0 && s < 256 && !have[s]) {
            have[s] = true;
            found[s] = next_k;
        }
        ++next_k;
    }
    return (seq_t)found[n];
}
