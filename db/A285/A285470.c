/*
 * A285470  Numbers k where "2" appears as the second digit of the decimal
 *          representation.
 * https://oeis.org/A285470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenate the first decimal digit of n, then the digit
 *            2, then the remaining digits of n. Equivalent recurrence: a(10*n
 *            + j) = 10*a(n) + j (0 ≤ j ≤ 9, n ≥ 1)
 * Range:     u8 n <= 29, u16 n <= 6999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"      /* seq_t pow10(idx_t e); */
#include "../utils/digit_count.h" /* uint64_t digit_count(uint64_t n); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1 */

    /* Number of decimal digits in n (1 … 20 for uint64_t). */
    idx_t k = digit_count(n);

    /* 10^(k-1) as an idx_t for extracting the first digit and the rest. */
    idx_t pow10_ll = 1;
    for (idx_t i = 1; i < k; ++i) {
        pow10_ll *= 10;
    }

    /* First digit d and the remaining part r. */
    idx_t d = n / pow10_ll;
    idx_t r = n % pow10_ll;

    /* Use the pow10 utility to get 10^k and 10^(k-1) as seq_t. */
    seq_t pow10_k     = pow10(k);
    seq_t pow10_k_1   = pow10(k - 1);

    /* Assemble the result. */
    return (seq_t)d * pow10_k + (seq_t)2 * pow10_k_1 + (seq_t)r;
}
