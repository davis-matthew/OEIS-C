/*
 * A281724  Numbers k with the property that (sum of digits of k) times
 *          (number of digits of k) equals (sum of {each digit of k} raised to
 *          the power {number of digits in k}).
 * https://oeis.org/A281724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) = n-th integer k (k >= 1) such that (sum of digits of k) *
 *            (number of digits of k) = sum of (each digit of k)^ (number of
 *            digits of k). This function enumerates all such k (the set is
 *            finite, last term 210) and returns the n-th term in ascending
 *            order
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   get_digits, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_digits.h"
#include "../utils/pow_uint.h"

static bool is_qualified(seq_t k)
{
    uint8_t digits[20];            /* enough for any feasible k */
    size_t len;
    get_digits(k, digits, &len);

    seq_t sum_digits = (seq_t)0;
    seq_t sum_powers = (seq_t)0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t d = digits[i];
        sum_digits += (seq_t)d;
        sum_powers += pow_uint(d, len);   /* digit^len */
    }

    seq_t lhs = sum_digits * (seq_t)len;   /* (sum of digits) * len */
    return lhs == sum_powers;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* The sequence is finite; the largest term known is 210.  */
    const idx_t MAX_K = 999;   /* safe upper bound for enumeration */

    idx_t count = 0;
    for (idx_t i = 1; i <= MAX_K; ++i) {
        seq_t k = (seq_t)i;
        if (is_qualified(k)) {
            ++count;
            if (count == n) {
                return k;           /* n-th term found */
            }
        }
    }
    /* If n exceeds the number of terms, return 0. */
    return (seq_t)0;
}
