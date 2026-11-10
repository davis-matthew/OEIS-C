/*
 * A316115  Numbers k such that A274016(k) = k - 1.
 * https://oeis.org/A316115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 1 with A274016(m+1) = m (search; the
 *            lexicographically first tuple for m+1 must have d*e*f = 1)
 * Range:     u8 n <= 42, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* does r = d*e*f exist with cmax >= d >= e >= f >= 1 ? */
static int A316115_has_def(uint64_t r, uint64_t cmax)
{
    for (uint64_t d = 1; d <= cmax; ++d) {
        if (d * d * d < r) continue;
        if (r % d) continue;
        uint64_t rem = r / d;
        for (uint64_t e = 1; e <= d && e <= rem; ++e) {
            if (e * e < rem) continue;
            if (rem % e == 0) return 1;
        }
    }
    return 0;
}

static int A316115_is_term(uint64_t m)
{
    uint64_t A = 0, B = 0, C = 0;
    bool found = false;
    for (uint64_t a = 1; a <= m && !found; ++a) {
        if (a * a * a < m) continue;
        if (m % a) continue;
        uint64_t q = m / a;
        for (uint64_t b = 1; b <= a && b <= q; ++b) {
            if (b * b < q) continue;
            if (q % b == 0) { A = a; B = b; C = q / b; found = true; break; }
        }
    }
    uint64_t lo = 1;
    while (2 * lo * lo * lo < m + 1) ++lo;
    for (uint64_t ap = lo; ap <= A; ++ap) {
        for (uint64_t bp = 1; bp <= ap; ++bp) {
            if (ap == A && bp > B) break;
            for (uint64_t cp = 1; cp <= bp; ++cp) {
                if (ap == A && bp == B && cp >= C) break;
                uint64_t P = ap * bp * cp;
                if (P > m) break;
                uint64_t r = m + 1 - P;
                if (r > cp * cp * cp) continue;
                if (A316115_has_def(r, cp)) return 0;
            }
        }
    }
    return 1;
}

seq_t a(idx_t n)
{
    static uint64_t terms[4096];
    static uint64_t cnt = 0;
    static uint64_t next_m = 1;
    if (n < 1 || n > 4096) return 0;
    while (cnt < (uint64_t)n) {
        if (A316115_is_term(next_m)) terms[cnt++] = next_m;
        ++next_m;
    }
    return (seq_t)terms[n - 1];
}
