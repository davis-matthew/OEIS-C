/*
 * A360788  Expansion of Sum_{k>=0} x^k / (1 - (k*x)^3)^(k+1).
 * https://oeis.org/A360788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (n-3*k)^(3*k) * binomial(n-2*k, k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t base = (seq_t)(n - 3 * k);
        seq_t exp  = (seq_t)(3 * k);
        seq_t pw;
        if (k == 0) {
            pw = (seq_t)1;
        } else {
            pw = ipow(base, exp);
        }
        seq_t bin = binomial(n - 2 * k, k);
        sum += bin * pw;
    }
    return sum;
}
