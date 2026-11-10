/*
 * A350866  a(n) = A010051(A339399(n)).
 * https://oeis.org/A350866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A010051(A339399(n)); A339399 lists the pairs (s,k-s),
 *            1<=s<=k-s, for k=2,3,...; found by walking the rows k, then
 *            testing primality
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-10)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t k = 2;
    idx_t m = n;
    while (m > 2 * (k / 2)) {
        m -= 2 * (k / 2);
        k++;
    }
    idx_t s = (m - 1) / 2 + 1;
    idx_t v = (m & 1) ? s : (k - s);
    return (seq_t)(is_prime(v) ? 1 : 0);
}
