/*
 * A393063  a(n) = A276085(LCM_{d|n, d<n} A276086(d)), where LCM is the least
 *          common multiple, A276086 is the primorial base exp-function, and
 *          A276085 is its left inverse.
 * https://oeis.org/A393063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = A276085(LCM_{d|n, d<n} A276086(d)). The LCM of the
 *            A276086(d) has, for each prime(k+1), exponent max_d
 *            (primorial-base digit k of d), so a(n) = Sum_k (max over proper
 *            divisors d of n of digit_k(d)) * prime(1)*...*prime(k)
 * Range:     u8 n <= 419, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 104 terms through n = 9535 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define PB_NPR 20

static uint64_t pb_primes[PB_NPR];
static bool pb_ready = false;

static void pb_init(void)
{
    if (pb_ready) return;
    int cnt = 0;
    uint64_t c = 2;
    while (cnt < PB_NPR) {
        bool isp = true;
        for (int i = 0; i < cnt; i++) {
            uint64_t p = pb_primes[i];
            if (p * p > c) break;
            if (c % p == 0) { isp = false; break; }
        }
        if (isp) pb_primes[cnt++] = c;
        c++;
    }
    pb_ready = true;
}

/* Update per-position maxima of primorial-base digits of d. */
static void pb_update(uint64_t d, unsigned *mx)
{
    int k = 0;
    while (d > 0 && k < PB_NPR) {
        unsigned dig = (unsigned)(d % pb_primes[k]);
        d /= pb_primes[k];
        if (dig > mx[k]) mx[k] = dig;
        k++;
    }
}

seq_t a(idx_t n)
{
    pb_init();
    unsigned mx[PB_NPR];
    int i;
    for (i = 0; i < PB_NPR; i++) mx[i] = 0;
    uint64_t N = (uint64_t)n;
    if (N <= 1) return (seq_t)0;
    for (uint64_t d = 1; d * d <= N; d++) {
        if (N % d != 0) continue;
        uint64_t e = N / d;
        if (d < N) pb_update(d, mx);
        if (e != d && e < N) pb_update(e, mx);
    }
    int top = -1;
    for (i = 0; i < PB_NPR; i++) if (mx[i]) top = i;
    seq_t result = 0;
    seq_t P = 1;
    for (i = 0; i <= top; i++) {
        result += (seq_t)mx[i] * P;
        if (i < top) P = P * (seq_t)pb_primes[i];
    }
    return result;
}
