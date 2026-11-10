/*
 * A272406  Primes p == 1 (mod 3) for which A261029(34*p) = 2.
 * https://oeis.org/A272406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th prime p == 1 (mod 3) with A261029(34*p) = 2;
 *            computed by a sieve-style count of x^3+y^3+z^3-3xyz up to
 *            34*1155
 * Range:     u8 n <= 25, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define A272406_LIM 39270

static unsigned A272406_terms[400];
static int A272406_cnt = -1;

static void A272406_build(void)
{
    static unsigned cnt[A272406_LIM + 1];
    int64_t L = A272406_LIM;
    for (int64_t x = 0; x <= L; ++x) {
        int64_t x3 = x * x * x;
        bool done = false;
        for (int64_t y = x; ; ++y) {
            int64_t z = (y > x + 1) ? y : x + 1;
            int64_t f = x3 + y * y * y + z * z * z - 3 * x * y * z;
            if (f > L) {
                if (y == x) done = true;
                break;
            }
            while (f <= L) {
                cnt[f]++;
                int64_t inc = 3 * z * z + 3 * z + 1 - 3 * x * y;
                ++z;
                f += inc;
            }
        }
        if (done) break;
    }
    int k = 0;
    for (unsigned p = 7; p < 1156; p += 3) {
        if (p % 3 != 1) continue;
        bool pr = true;
        for (unsigned d = 2; d * d <= p; ++d)
            if (p % d == 0) { pr = false; break; }
        if (!pr) continue;
        if (cnt[34 * p] == 2) A272406_terms[k++] = p;
    }
    A272406_cnt = k;
}

seq_t a(idx_t n)
{
    if (A272406_cnt < 0) A272406_build();
    if (n < 1 || n > (idx_t)A272406_cnt) return 0;
    return (seq_t)A272406_terms[n - 1];
}
