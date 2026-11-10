/*
 * A331279  a(n) = A122111(A006047(n)).
 * https://oeis.org/A331279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A122111(2^r*3^s), r = # of 1's, s = # of 2's in base 3 of
 *            n; equals prime(r+s)*prime(s) if s>0, prime(r) if s=0 (1 if
 *            r=s=0)
 * Range:     u8 n <= 2185, u16 n <= 19683, u32 n <= 19683, u64 n <= 19683,
 *            u128 n <= 19683, big any n
 * Validated: standard, pass, 111 terms through n = 9679 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A331279_kth_prime(uint64_t k)
{
    uint64_t cnt = 0, c = 1;
    while (cnt < k) {
        ++c;
        int ok = 1;
        for (uint64_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = 0; break; }
        }
        if (ok) ++cnt;
    }
    return c;
}

seq_t a(idx_t n)
{
    uint64_t r = 0, s = 0;
    idx_t t = n;
    while (t > 0) {
        unsigned d = (unsigned)(t % 3);
        if (d == 1) ++r;
        else if (d == 2) ++s;
        t /= 3;
    }
    if (r == 0 && s == 0) return (seq_t)1;
    if (s == 0) return (seq_t)A331279_kth_prime(r);
    return (seq_t)A331279_kth_prime(r + s) * (seq_t)A331279_kth_prime(s);
}
