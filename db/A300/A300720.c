/*
 * A300720  Difference between A010848 and its Möbius transform.
 * https://oeis.org/A300720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -Sum_{d|n, d<n} mu(n/d)*A010848(d), A010848(d) = d -
 *            floor(d/rad(d))
 * Range:     u8 n <= 377, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 107 terms through n = 9648 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static idx_t A010848_f(idx_t n) {
    if (n <= 1) return 0;
    idx_t rad = 1, temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            rad *= p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) rad *= temp;
    return n - n / rad;
}

static int A008683_mu(idx_t n) {
    if (n == 1) return 1;
    idx_t tmp = n;
    int cnt = 0;
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            ++cnt;
            tmp /= p;
            if (tmp % p == 0) return 0;
        }
    }
    if (tmp > 1) ++cnt;
    return (cnt & 1) ? -1 : 1;
}

static void A300720_acc(idx_t n, idx_t d, seq_t *pos, seq_t *neg) {
    idx_t m = n / d;
    if (m <= 1) return;
    int mu = A008683_mu(m);
    if (mu == 0) return;
    seq_t v = (seq_t)A010848_f(d);
    if (mu > 0) *pos += v; else *neg += v;
}

seq_t a(idx_t n) {
    seq_t pos = 0, neg = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            A300720_acc(n, i, &pos, &neg);
            idx_t j = n / i;
            if (j != i) A300720_acc(n, j, &pos, &neg);
        }
    }
    return neg - pos;
}
