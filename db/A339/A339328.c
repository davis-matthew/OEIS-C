/*
 * A339328  Integers m such that A240471(m) > A115588(m).
 * https://oeis.org/A339328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer m>1 with floor(m*d(m)/sigma(m)) >
 *            A115588(m); found by direct search
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static void A339328_collect(uint64_t v, uint64_t *seen, int *cnt)
{
    if (v <= 1) return;
    uint64_t x = v;
    for (uint64_t p = 2; p * p <= x; p += (p == 2 ? 1 : 2)) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) { x /= p; ++e; }
            bool present = false;
            for (int j = 0; j < *cnt; ++j) if (seen[j] == p) { present = true; break; }
            if (!present && *cnt < 128) seen[(*cnt)++] = p;
            if (e > 1) A339328_collect(e, seen, cnt);
        }
    }
    if (x > 1) {
        bool present = false;
        for (int j = 0; j < *cnt; ++j) if (seen[j] == x) { present = true; break; }
        if (!present && *cnt < 128) seen[(*cnt)++] = x;
    }
}

static bool A339328_test(uint64_t m)
{
    uint64_t seen[128];
    int cnt = 0;
    uint64_t x = m, d = 1, sg = 1;
    for (uint64_t p = 2; p * p <= x; p += (p == 2 ? 1 : 2)) {
        if (x % p == 0) {
            uint64_t e = 0, s = 1, pw = 1;
            while (x % p == 0) { x /= p; ++e; pw *= p; s += pw; }
            d *= (e + 1);
            sg *= s;
            bool present = false;
            for (int j = 0; j < cnt; ++j) if (seen[j] == p) { present = true; break; }
            if (!present && cnt < 128) seen[cnt++] = p;
            if (e > 1) A339328_collect(e, seen, &cnt);
        }
    }
    if (x > 1) {
        d *= 2;
        sg *= (x + 1);
        bool present = false;
        for (int j = 0; j < cnt; ++j) if (seen[j] == x) { present = true; break; }
        if (!present && cnt < 128) seen[cnt++] = x;
    }
    unsigned __int128 num = (unsigned __int128)m * d;
    uint64_t hm = (uint64_t)(num / sg);
    return hm > (uint64_t)cnt;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t m = 1;
    while (count < (uint64_t)n) {
        ++m;
        if (A339328_test(m)) ++count;
    }
    return (seq_t)m;
}
