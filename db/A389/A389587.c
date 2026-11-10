/*
 * A389587  Increasing partition array based on the fractions
 *          A168265(n)/A389810(n); see Comments.
 * https://oeis.org/A389587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   search/simulation. Fractions f(m)=A168265(m)/A389810(m)
 *            (1/2,1/3,2/3,1/5,...); increasing partition array: row r
 *            greedily takes, scanning m upward, each unused m with f(m) >
 *            f(previous entry of the row); a(n) read by antidiagonals
 *            p(1,k),...,p(k,1)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool a389587_isp(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t d = 1;
    while ((uint64_t)d * (d + 1) / 2 < (uint64_t)n) d++;
    uint64_t j = (uint64_t)n - d * (d - 1) / 2;   /* row */
    uint64_t c = d + 1 - j;                       /* column */
    uint64_t M = 64;
    for (;;) {
        uint64_t *num = (uint64_t *)calloc(M + 1, sizeof(uint64_t));
        uint64_t *den = (uint64_t *)calloc(M + 1, sizeof(uint64_t));
        unsigned char *used = (unsigned char *)calloc(M + 1, 1);
        if (!num || !den || !used) abort();
        uint64_t q = 2, i = 1;
        for (uint64_t m = 1; m <= M; m++) {
            num[m] = i; den[m] = q;
            i++;
            if (i == q) {
                q++;
                while (!a389587_isp(q)) q++;
                i = 1;
            }
        }
        uint64_t result = 0;
        for (uint64_t row = 1; row <= j; row++) {
            uint64_t last = 0, count = 0;
            for (uint64_t m = 1; m <= M; m++) {
                if (used[m]) continue;
                if (count == 0 || num[m] * den[last] > num[last] * den[m]) {
                    used[m] = 1;
                    last = m;
                    count++;
                    if (row == j && count == c) { result = m; break; }
                }
            }
            if (row == j) break;
        }
        free(num); free(den); free(used);
        if (result) return (seq_t)result;
        M *= 2;
    }
}
