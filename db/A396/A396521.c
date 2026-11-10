/*
 * A396521  Positions of terms > 3 in A200811.
 * https://oeis.org/A396521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index k of the n-th composite whose iterated-tau chain
 *            ends at an odd prime > 3 (A200811(k) > 3); computed by
 *            search/scan
 * Range:     u8 n <= 11, u16 n <= 7772, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9990 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A396521_tau(uint64_t m)
{
    uint64_t t = 1;
    if ((m & 1) == 0) {
        uint64_t e = 0;
        while ((m & 1) == 0) { m >>= 1; ++e; }
        t *= (e + 1);
    }
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            uint64_t e = 0;
            while (m % d == 0) { m /= d; ++e; }
            t *= (e + 1);
        }
    }
    if (m > 1) t *= 2;
    return t;
}

static bool A396521_oddprime(uint64_t t)
{
    if (t < 3 || (t & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= t; d += 2)
        if (t % d == 0) return false;
    return true;
}

static uint64_t *A396521_found = NULL;
static size_t A396521_cnt = 0, A396521_cap = 0;
static uint64_t A396521_m = 3;   /* last integer scanned */
static uint64_t A396521_ci = 0;  /* composites seen so far */

seq_t a(idx_t n)
{
    while (A396521_cnt < (size_t)n) {
        ++A396521_m;
        uint64_t t = A396521_tau(A396521_m);
        if (t == 2) continue;
        ++A396521_ci;
        while (!A396521_oddprime(t))
            t = A396521_tau(t);
        if (t > 3) {
            if (A396521_cnt == A396521_cap) {
                A396521_cap = A396521_cap ? A396521_cap * 2 : 1024;
                A396521_found = (uint64_t *)realloc(A396521_found, A396521_cap * sizeof(uint64_t));
                if (!A396521_found) abort();
            }
            A396521_found[A396521_cnt++] = A396521_ci;
        }
    }
    return (seq_t)A396521_found[n - 1];
}
