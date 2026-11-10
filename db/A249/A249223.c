/*
 * A249223  Triangle read by rows: row n gives partial alternating sums of row
 *          n of A237048.
 * https://oeis.org/A249223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = Sum_{j=1..k} (-1)^(j+1)*A237048(n,j),
 *            1<=k<=floor((sqrt(8n+1)-1)/2); A237048(n,j)=1 iff n>=j(j+1)/2
 *            and j divides n-j(j+1)/2
 * Range:     u8 n <= 1600, u16 n <= 1600, u32 n <= 1600, u64 n <= 1600,
 *            u128 n <= 1600, big any n
 * Validated: standard, pass, 110 terms through n = 1541 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t row = 0, len = 0, next_t = 1, cum = 0;
    while (cum < n) {
        row++;
        if (row >= next_t) {
            len++;
            next_t = (len + 1) * (len + 2) / 2;
        }
        cum += len;
    }
    idx_t k = n - (cum - len);
    long long s = 0;
    for (idx_t j = 1; j <= k; j++) {
        idx_t st = j * (j + 1) / 2;
        if (row >= st && (row - st) % j == 0) {
            if (j & 1) s++; else s--;
        }
    }
    return (seq_t)s;
}
