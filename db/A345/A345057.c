/*
 * A345057  Distance to the next larger number k > n for which A011772(k) =
 *          A011772(n), or 0 if no such number exists.
 * https://oeis.org/A345057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A345056(n) - n, where A345056(n) is the least k > n with
 *            A011772(k) = A011772(n) (else a(n) = 0). The code finds it by
 *            enumerating the divisors of T(m), m = A011772(n), and computing
 *            A011772 via CRT
 * Range:     u8 n <= 15, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A345057_inv(seq_t a, seq_t b)
{
    if (b == 1) return 0;
    seq_t r0 = b, r1 = a % b, t0 = 0, t1 = 1;
    while (r1 != 0) {
        seq_t q = r0 / r1;
        seq_t r2 = r0 - q * r1;
        seq_t t2 = (t0 + b - (q % b) * t1 % b) % b;
        r0 = r1; r1 = r2; t0 = t1; t1 = t2;
    }
    return t0;
}

/* A011772: smallest m >= 1 with k | m(m+1)/2, via CRT over coprime splits of 2k */
static seq_t A345057_f(seq_t k)
{
    seq_t N = k * 2;
    seq_t pp[40];
    int cnt = 0;
    seq_t x = N;
    for (seq_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            seq_t q = 1;
            while (x % p == 0) { x /= p; q *= p; }
            pp[cnt++] = q;
        }
    }
    if (x > 1) pp[cnt++] = x;
    seq_t best = N - 1;
    if (best < 1) best = 1;
    for (unsigned mask = 0; mask < (1u << cnt); ++mask) {
        seq_t a = 1;
        for (int i = 0; i < cnt; ++i)
            if (mask & (1u << i)) a *= pp[i];
        seq_t b = N / a;
        seq_t m;
        if (b == 1) {
            m = N;
        } else {
            seq_t t = ((b - 1) % b) * A345057_inv(a % b, b) % b;
            m = a * t;
            if (m == 0) m = N;
        }
        if (m < best) best = m;
    }
    return best;
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t m = A345057_f(sn);
    seq_t T = m * (m + 1) / 2;
    seq_t best = 0;
    bool found = false;
    for (seq_t i = 1; i * i <= T; ++i) {
        if (T % i != 0) continue;
        seq_t c[2];
        c[0] = i;
        c[1] = T / i;
        for (int j = 0; j < 2; ++j) {
            seq_t k = c[j];
            if (k > sn && (!found || k < best)) {
                if (A345057_f(k) == m) { best = k; found = true; }
            }
        }
    }
    if (!found) return 0;
    return best - sn;
}
