/*
 * A342015  a(n) = A003415(A276086(n)) mod A276086(n).
 * https://oeis.org/A342015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   with m = A276086(n) = Prod prime(k+1)^d_k (d_k = primorial-base
 *            digits of n), a(n) = (Sum_k d_k * m/prime(k+1)) mod m, since m'
 *            = m*Sum e_i/p_i. a(0) = 0
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 417, u64 n <= 2308,
 *            u128 n <= 2310, big any n
 * Validated: standard, pass, 86 terms through n = 2213 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A342015_next_prime(uint64_t p)
{
    uint64_t c = p + 1;
    for (;; ++c) {
        bool ok = true;
        for (uint64_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok && c >= 2) return c;
    }
}

seq_t a(idx_t n)
{
    uint64_t ps[64];
    uint64_t ds[64];
    int cnt = 0;
    uint64_t rem = (uint64_t)n;
    uint64_t p = 2;
    seq_t m = 1;

    while (rem > 0 && cnt < 64) {
        uint64_t d = rem % p;
        rem /= p;
        ps[cnt] = p;
        ds[cnt] = d;
        cnt++;
        for (uint64_t j = 0; j < d; ++j)
            m = m * (seq_t)p;
        p = A342015_next_prime(p);
    }

    if (m <= 1)
        return 0;

    seq_t r = 0;
    for (int i = 0; i < cnt; ++i) {
        if (ds[i] == 0) continue;
        seq_t term = (m / (seq_t)ps[i]) * (seq_t)ds[i];  /* < m */
        if (r >= m - term)
            r = r - (m - term);
        else
            r = r + term;
    }
    return r;
}
