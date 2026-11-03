/*
 * A323409  Greatest common divisor of Product (p_i^e_i)-1 and n, when n =
 *          Product (p_i^e_i); a(n) = gcd(n, A047994(n)).
 * https://oeis.org/A323409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd( n , ∏_{p^e∥n} (p^e – 1) )
 * Range:     u8 n <= 1403, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9770 (2026-11-01)
 * Helpers:   factor_u64, gcd, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    size_t cnt;
    uint64_t pr[20], ex[20];
    factor_u64(n, pr, ex, &cnt);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = ipow_idx((seq_t)pr[i], (idx_t)ex[i]) - (seq_t)1;
        prod = prod * term;
    }

    return gcd((seq_t)n, prod);
}
