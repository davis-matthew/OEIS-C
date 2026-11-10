/*
 * A297194  Left-hand half of triangle A297193.
 * https://oeis.org/A297194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(k,j), n = k(k+1)/2 + j, 0<=j<=k, where T(k,j) is
 *            coefficient j of R_k(x), R_0 = 1, R_1 = 1+4x+x^2, R_k =
 *            (1+4x+x^2)*R_{k-1} - x^2*R_{k-2} (left half of triangle A297193)
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 51 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t k = 0;
    while ((k + 1) * (k + 2) / 2 <= n) k++;
    idx_t j = n - k * (k + 1) / 2;

    size_t sz = (size_t)(2 * k + 3);
    seq_t *p2 = mem_new(seq_t, sz);
    seq_t *p1 = mem_new(seq_t, sz);
    seq_t *cur = mem_new(seq_t, sz);

    p1[0] = (seq_t)1; /* R_0 */
    for (idx_t r = 1; r <= k; ++r) {
        idx_t len = 2 * r + 1;
        idx_t lenp = 2 * (r - 1) + 1;
        for (idx_t i = 0; i < len; ++i) {
            seq_t s = (seq_t)0;
            for (idx_t t = 0; t < 3; ++t) {
                if (i >= t && i - t < lenp) {
                    seq_t c = (t == 1) ? (seq_t)4 : (seq_t)1;
                    s += p1[i - t] * c;
                }
            }
            if (r >= 2 && i >= 2 && i - 2 < 2 * (r - 2) + 1)
                s -= p2[i - 2];
            cur[i] = s;
        }
        seq_t *tmp = p2;
        p2 = p1;
        p1 = cur;
        cur = tmp;
    }
    seq_t res = p1[j];
    mem_free(p2);
    mem_free(p1);
    mem_free(cur);
    return res;
}
