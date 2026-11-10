/*
 * A372290  Numbers that occur in the odd bisection of A371094.
 * https://oeis.org/A372290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest distinct value of 4^i*c + (4^i-1)/3 over
 *            i>=1, with c = 6k-1 (i odd) or c = 6k-5 (i even), k>=1 (the
 *            values of array A371100); computed by marking values up to
 *            24n+24 and scanning
 * Range:     u8 n <= 10, u16 n <= 2910, u32 n <= 62122, u64 n <= 62122,
 *            u128 n <= 62122, big any n
 * Validated: standard, pass, 77 terms through n = 9782 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    unsigned long long N = (unsigned long long)n;
    unsigned long long X = 24ULL * N + 24ULL;
    unsigned char *mark = (unsigned char *)calloc((size_t)X + 1, 1);
    if (!mark) return (seq_t)0;

    unsigned long long p = 4ULL;
    for (int i = 1; ; i++, p *= 4ULL) {
        unsigned long long add = (p - 1) / 3ULL;
        unsigned long long c = (i & 1) ? 5ULL : 1ULL;
        unsigned long long v = p * c + add;
        if (v > X) break;
        unsigned long long step = 6ULL * p;
        while (v <= X) {
            mark[v] = 1;
            v += step;
        }
    }

    unsigned long long cnt = 0, res = 0;
    for (unsigned long long v = 0; v <= X; v++) {
        if (mark[v]) {
            cnt++;
            if (cnt == N) { res = v; break; }
        }
    }
    free(mark);
    return (seq_t)res;
}
