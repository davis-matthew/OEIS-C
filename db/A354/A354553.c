/*
 * A354553  Expansion of e.g.f. exp( x * exp(x^3) ).
 * https://oeis.org/A354553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (n - 3*k)^k/(k! * (n - 3*k)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   factorial_ratio, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow_seq.h"
#include "../utils/factorial_ratio.h"

seq_t a(idx_t n)
{
    idx_t kmax = n / (idx_t)3;
    seq_t total = (seq_t)0;
    for (idx_t k = (idx_t)0; k <= kmax; ++k) {
        idx_t n_minus_3k = n - (idx_t)3 * k;
        if (n_minus_3k == (idx_t)0 && k > (idx_t)0)
            continue;
        seq_t ratio = factorial_ratio(n, k);
        seq_t power = pow_seq((seq_t)n_minus_3k, (int)k);
        total += ratio * power;
    }
    return total;
}
