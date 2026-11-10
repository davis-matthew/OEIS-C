/*
 * A344760  "Inverted variant" of A342002: a(n) = A342002(A289234(n)).
 * https://oeis.org/A344760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A342002(A289234(n)); with n = sum d_i * (p_0*...*p_{i-1})
 *            in primorial base (0<=d_i<p_i), e_i = d_i^(-1) mod p_i (0 if
 *            d_i=0), rad = prod of p_i with e_i>0, a(n) = sum e_i * rad/p_i
 * Range:     u8 n <= 40, u16 n <= 2578, u32 n <= 13860, u64 n <= 13860,
 *            u128 n <= 13860, big any n
 * Validated: standard, pass, 95 terms through n = 9941 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const unsigned A344760_pr[] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
    31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u
};

seq_t a(idx_t n)
{
    unsigned e[20];
    size_t cnt = 0;
    seq_t rad = 1;
    idx_t t = n;
    while (t > 0 && cnt < 20) {
        unsigned p = A344760_pr[cnt];
        unsigned d = (unsigned)(t % p);
        t /= p;
        unsigned inv = 0;
        if (d > 0) {
            for (unsigned x = 1; x < p; ++x) {
                if ((x * d) % p == 1u) { inv = x; break; }
            }
            rad *= (seq_t)p;
        }
        e[cnt++] = inv;
    }
    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (e[i] == 0) continue;
        result += (rad / (seq_t)A344760_pr[i]) * (seq_t)e[i];
    }
    return result;
}
