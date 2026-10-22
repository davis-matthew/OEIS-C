/*
 * A357171  a(n) is the number of divisors of n whose digits are in strictly
 *          increasing order (A009993).
 * https://oeis.org/A357171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of n whose decimal digits are in
 *            strictly increasing order (OEIS A357171)
 * Range:     u8 n <= 9, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   build_inc_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_inc_digits.h"

static seq_t *inc_digits;
static size_t inc_count;

static void init_inc_digits(void) {
    if (inc_digits != NULL) return;          /* already initialized */
    inc_digits = (seq_t *)malloc(sizeof(seq_t) * 512);
    size_t cnt = 0;
    for (int d = 1; d <= 9; ++d) {
        build_inc_digits(inc_digits, &cnt, (seq_t)d, d);
    }
    inc_count = cnt;                          /* should be 511 */
}

static bool is_inc_digit(seq_t x) {
    for (size_t i = 0; i < inc_count; ++i) {
        if (inc_digits[i] == x) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* out of defined offset */
    init_inc_digits();
    seq_t result = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            seq_t d1 = (seq_t)i;
            seq_t d2 = (seq_t)(n / i);
            if (is_inc_digit(d1)) ++result;
            if (d1 != d2 && is_inc_digit(d2)) ++result;
        }
    }
    return result;
}
