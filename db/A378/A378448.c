/*
 * A378448  Characteristic function of A064771: a(n) = 1 if there is exactly
 *          one subset of the proper divisors of n that sum to n, otherwise 0.
 * https://oeis.org/A378448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if there is exactly one subset of the proper divisors
 *            of n that sum to n, otherwise 0
 * Range:     u8 n <= 8971, u16 n <= 8971, u32 n <= 8971, u64 n <= 8971,
 *            u128 n <= 8971, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of offset, return 0 */

    /* Collect proper divisors of n.  For n ≤ 10000 the number of divisors is < 256. */
    idx_t divs[256];
    size_t m = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if (d1 < n) divs[m++] = d1;
            if (d2 != d1 && d2 < n) divs[m++] = d2;
        }
    }

    /* DP: count subsets that sum to each value up to n, capped at 2. */
    size_t ns = (size_t)n;
    uint8_t *cnt = (uint8_t *)calloc(ns + 1, sizeof(uint8_t));
    if (!cnt) return (seq_t)0;                 /* allocation failed */
    cnt[0] = 1;                                

    for (size_t k = 0; k < m; ++k) {
        idx_t d = divs[k];
        for (idx_t s = n; s >= d; --s) {
            if (cnt[(size_t)(s - d)]) {
                cnt[(size_t)s] += cnt[(size_t)(s - d)];
                if (cnt[(size_t)s] > 1) cnt[(size_t)s] = 2;
            }
        }
    }

    seq_t res = (cnt[ns] == 1) ? (seq_t)1 : (seq_t)0;
    free(cnt);
    return res;
}
