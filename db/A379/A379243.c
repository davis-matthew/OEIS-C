/*
 * A379243  a(n) = (10^(n + 1) + 10^(n - min{v_2(n), v_5(n)}) + 1)^n, where
 *          v_p(n) indicates the p-adic valuation of n.
 * https://oeis.org/A379243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (10^(n + 1) + 10^(n - min{v_2(n), v_5(n)}) + 1)^n where
 *            v_p(n) is the p‑adic valuation of n
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 31 (2026-11-01)
 * Helpers:   ipow, valuation_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/valuation_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset starts at 1, but guard for 0 */
    idx_t v2 = valuation_idx(n, 2);
    idx_t v5 = valuation_idx(n, 5);
    idx_t k = v2 < v5 ? v2 : v5;         /* min{v_2(n), v_5(n)} */

    seq_t base = pow10(n + 1) + pow10(n - k) + 1;
    seq_t exp  = (seq_t)n;
    return ipow(base, exp);
}
