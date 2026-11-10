/*
 * A336922  a(n) = A331410(n) - A005087(n).
 * https://oeis.org/A336922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A331410(n) - A005087(n), where A331410 counts steps of k
 *            -> k + k/lpf(k) to reach a power of 2, and A005087 counts
 *            distinct odd prime factors of n
 * Range:     u8 n <= 393, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8580 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t A331410_lpf(seq_t m)
{
    seq_t best = 1;
    while ((m & 1) == 0) {
        best = 2;
        m >>= 1;
    }
    for (seq_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            best = d;
            while (m % d == 0)
                m /= d;
        }
    }
    if (m > 1)
        best = m;
    return best;
}

static seq_t A331410_a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t curr = (seq_t)n;
    seq_t cnt = 0;
    while ((curr & (curr - 1)) != 0) {
        seq_t p = A331410_lpf(curr);
        curr += curr / p;
        ++cnt;
    }
    return cnt;
}

static seq_t A005087_a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t count = 0;
    idx_t t = n;
    while (t % 2 == 0)
        t /= 2;
    for (idx_t i = 3; i * i <= t; i += 2) {
        if (t % i == 0) {
            count++;
            while (t % i == 0)
                t /= i;
        }
    }
    if (t > 1)
        count++;
    return count;
}

seq_t a(idx_t n)
{
    return A331410_a(n) - A005087_a(n);
}
