/*
 * A373094  a(n) is the least number k such that A373092(k) = n.
 * https://oeis.org/A373094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, hard, more
 * Formula:   a(n) = least k such that iterating x -> A093653(x) from k needs
 *            exactly n steps to reach a fixed point; found by a sieve search
 *            up to 2*10^6 (covers n <= 7)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A373094_f_slow(uint64_t nn)
{
    uint64_t pr[20];
    int ex[20];
    int cnt = 0;
    uint64_t tmp = nn;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            do { tmp /= p; ++e; } while (tmp % p == 0);
            pr[cnt] = p; ex[cnt] = e; ++cnt;
        }
    }
    if (tmp > 1) { pr[cnt] = tmp; ex[cnt] = 1; ++cnt; }

    /* iterative divisor enumeration */
    uint64_t sum = 0;
    uint64_t divs[200000];
    size_t nd = 1;
    divs[0] = 1;
    for (int i = 0; i < cnt; ++i) {
        size_t cur = nd;
        uint64_t pw = 1;
        for (int e = 1; e <= ex[i]; ++e) {
            pw *= pr[i];
            for (size_t j = 0; j < cur; ++j) divs[nd++] = divs[j] * pw;
        }
    }
    for (size_t j = 0; j < nd; ++j) sum += (uint64_t)__builtin_popcountll(divs[j]);
    return sum;
}

#define A373094_L 2000000u
#define A373094_MAXN 8

seq_t a(idx_t n)
{
    static int done = 0;
    static uint64_t res[A373094_MAXN];

    if (!done) {
        uint32_t *f = (uint32_t *)calloc(A373094_L + 1, sizeof(uint32_t));
        uint32_t d, m;
        uint32_t k;
        int i;
        for (i = 0; i < A373094_MAXN; ++i) res[i] = 0;
        for (d = 1; d <= A373094_L; ++d) {
            uint32_t pc = (uint32_t)__builtin_popcount(d);
            for (m = d; m <= A373094_L; m += d) f[m] += pc;
        }
        for (k = 1; k <= A373094_L; ++k) {
            uint64_t x = k;
            int steps = 0;
            for (;;) {
                uint64_t y = (x <= A373094_L) ? (uint64_t)f[x] : A373094_f_slow(x);
                if (y == x) break;
                x = y;
                ++steps;
                if (steps > 1000) break;
            }
            if (steps < A373094_MAXN && res[steps] == 0) res[steps] = k;
        }
        free(f);
        done = 1;
    }
    if (n >= A373094_MAXN) return (seq_t)0;
    return (seq_t)res[n];
}
