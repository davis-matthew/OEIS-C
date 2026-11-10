/*
 * A336921  a(n) = A331410(n) - A087436(n).
 * https://oeis.org/A336921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A331410(n) - A087436(n), where A331410(n) counts
 *            iterations of k -> k + k/lpf(k) needed to reach a power of 2,
 *            and A087436(n) is the number of odd prime factors of n counted
 *            with multiplicity
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9263 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A336921_lpf(uint64_t m)
{
    uint64_t best = 1;
    while ((m & 1ULL) == 0) { m >>= 1; best = 2; }
    for (uint64_t p = 3; p <= m / p; p += 2) {
        while (m % p == 0) { m /= p; best = p; }
    }
    if (m > 1) best = m;
    return best;
}

static uint64_t A336921_iters(uint64_t n)
{
    uint64_t k = n, cnt = 0;
    while (k > 1 && (k & (k - 1)) != 0) {
        uint64_t p = A336921_lpf(k);
        k += k / p;
        ++cnt;
    }
    return cnt;
}

static uint64_t A336921_oddomega(uint64_t m)
{
    if (m <= 1) return 0;
    while ((m & 1ULL) == 0) m >>= 1;
    uint64_t cnt = 0;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        while (m % p == 0) { m /= p; ++cnt; }
    }
    if (m > 1) ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    uint64_t x = A336921_iters((uint64_t)n);
    uint64_t y = A336921_oddomega((uint64_t)n);
    return (seq_t)(x - y);
}
