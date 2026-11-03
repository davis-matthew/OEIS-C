/*
 * A242449  a(n) = Sum_{k=0..n} C(n,k) * (2*k+1)^(2*n+1).
 * https://oeis.org/A242449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * (2*k+1)^(2*n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 188 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t exp = (idx_t)(2 * n + 1);
    for (idx_t k = 0; k <= n; ++k) {
        seq_t coeff = binomial(n, k);
        seq_t base = (seq_t)(2 * k + 1);
        seq_t pw = ipow(base, exp);
        result += coeff * pw;
    }
    return result;
}
