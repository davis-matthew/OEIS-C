/*
 * A301803  a(n) is the number of n-digit numbers divisible by their last
 *          digit.
 * https://oeis.org/A301803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{d=1}^{9} ( ⌊(10^{n-1}-1)/(d/gcd(10,d))⌋ -
 *            ⌊(10^{n-2}-1)/(d/gcd(10,d))⌋ ) computed directly without any
 *            hidden tables
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* For a single‑digit number every digit 1…9 is divisible by itself. */
    if (n == 1)
        return (seq_t)9;

    /* Step values s[d] = d/gcd(10,d) for d=1..9 */
    static const idx_t step[10] = {0, 1, 1, 3, 2, 1, 3, 7, 4, 9};

    /* 10^(n-1) and 10^(n-2) as big integers */
    seq_t pow_n1 = pow10(n - 1);   /* 10^(n-1) */
    seq_t pow_n0 = pow10(n - 2);   /* 10^(n-2) */

    seq_t total = 0;
    for (idx_t d = 1; d <= 9; ++d) {
        seq_t step_d = (seq_t)step[d];
        seq_t part1 = (pow_n1 - (seq_t)1) / step_d;
        seq_t part0 = (pow_n0 - (seq_t)1) / step_d;
        total += part1 - part0;
    }
    return total;
}
