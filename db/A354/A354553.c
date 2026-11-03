/*
 * A354553  Expansion of e.g.f. exp( x * exp(x^3) ).
 * https://oeis.org/A354553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (n - 3*k)^k/(k! * (n - 3*k)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 471 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

static seq_t factorial_ratio(idx_t n, idx_t k)
{
    if (k == (idx_t)0)
        return (seq_t)1;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        idx_t base = n - (idx_t)3 * (i - (idx_t)1);
        res *= (seq_t)base;
        res *= (seq_t)(base - (idx_t)1);
        res *= (seq_t)(base - (idx_t)2);
        res /= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t kmax = n / (idx_t)3;
    seq_t total = (seq_t)0;
    for (idx_t k = (idx_t)0; k <= kmax; ++k) {
        idx_t n_minus_3k = n - (idx_t)3 * k;
        if (n_minus_3k == (idx_t)0 && k > (idx_t)0)
            continue;
        seq_t ratio = factorial_ratio(n, k);
        seq_t power = ipow_i32((seq_t)n_minus_3k, (int)k);
        total += ratio * power;
    }
    return total;
}
