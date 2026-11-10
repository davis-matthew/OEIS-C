/*
 * A322316  Lexicographically earliest such sequence a that a(i) = a(j) =>
 *          A122841(i) = A122841(j) and A244417(i) = A244417(j), for all i, j.
 * https://oeis.org/A322316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = RGS transform of [A122841(n), A244417(n)]; with
 *            lo=min(v2,v3), hi=max(v2,v3), F=2^hi*3^lo (first n with that
 *            pair), a(n) = #{(x>=y>=0): 2^x*3^y <= F}
 * Range:     u8 n <= 19683, u16 n <= 19683, u32 n <= 19683, u64 n <= 19683,
 *            u128 n <= 19683, big any n
 * Validated: standard, pass, 121 terms through n = 9695 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    uint64_t t = (uint64_t)n;
    unsigned v2 = 0, v3 = 0;
    while ((t & 1ULL) == 0ULL) { t >>= 1; v2++; }
    t = (uint64_t)n;
    while (t % 3ULL == 0ULL) { t /= 3; v3++; }
    unsigned lo = v2 < v3 ? v2 : v3;
    unsigned hi = v2 < v3 ? v3 : v2;

    uint64_t F = 1;
    for (unsigned i = 0; i < hi; i++) F *= 2;
    for (unsigned i = 0; i < lo; i++) F *= 3;

    uint64_t count = 0;
    uint64_t s6 = 1;
    for (;;) {
        uint64_t v = s6;
        for (;;) {
            if (v > F) break;
            count++;
            if (v > F / 2) break;
            v *= 2;
        }
        if (s6 > F / 6) break;
        s6 *= 6;
    }
    return (seq_t)count;
}
