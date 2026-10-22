/*
 * A356444  Number of ways to create an angle excess of n degrees using 3
 *          regular polygons with integral internal angles.
 * https://oeis.org/A356444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered triples of divisors of 360 (each ≥ 3)
 *            whose internal angles sum to n+360 degrees. Equivalently: count
 *            triples a,b,c (a≥b≥c), a,b,c|360, such that 360/a + 360/b +
 *            360/c = 180 − n. For n<1 or n>177 the value is 0. Implementation
 *            uses brute‑force over the 22 divisors of 360
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const idx_t _divisors[] = {
    3, 4, 5, 6, 8, 9, 10, 12, 15, 18, 20, 24,
    30, 36, 40, 45, 60, 72, 90, 120, 180, 360
};
static const idx_t _div_len =
    (idx_t)(sizeof _divisors / sizeof * _divisors);

static seq_t _cnt[178] = {0};   /* indices 1..177 used; index 0 unused */
static bool _initialized = false;

static void _init_counts(void)
{
    if (_initialized)
        return;
    for (idx_t i = 0; i < _div_len; ++i) {
        for (idx_t j = i; j < _div_len; ++j) {
            for (idx_t k = j; k < _div_len; ++k) {
                int sum = 360 / _divisors[i] +
                          360 / _divisors[j] +
                          360 / _divisors[k];
                int n = 180 - sum;
                if (n >= 1 && n <= 177)
                    ++_cnt[(idx_t)n];
            }
        }
    }
    _initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0 || n > 177)
        return 0;
    _init_counts();
    return _cnt[(idx_t)n];
}
