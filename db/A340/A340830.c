/*
 * A340830  Number of strict integer partitions of n such that every part is a
 *          multiple of the number of parts.
 * https://oeis.org/A340830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A008289(n/d, d)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t _dp_nmax = 0;          /* maximum n processed so far */
static size_t _dp_kmax = 0;        /* maximum k needed for _dp_nmax */
static seq_t *_dp_array = NULL;    /* flattened DP table: dp[sum * cols + k] */

/* Build (or refresh) DP table up to Nmax */
static void _build_dp(idx_t Nmax) {
    if (Nmax <= _dp_nmax) return;          /* already large enough */

    /* free previous table */
    if (_dp_array != NULL) {
#ifdef __cplusplus
        delete[] _dp_array;
#else
        free(_dp_array);
#endif
        _dp_array = NULL;
    }

    /* compute maximum k such that 1+2+...+k <= Nmax */
    size_t kmax = 0;
    while ((kmax + 1) * (kmax + 2) / 2 <= Nmax) ++kmax;
    _dp_kmax = kmax;

    size_t rows = (size_t)(Nmax + 1);
    size_t cols = _dp_kmax + 1;
    size_t total = rows * cols;

#ifdef __cplusplus
    _dp_array = new seq_t[total];
    std::fill(_dp_array, _dp_array + total, (seq_t)0);
#else
    _dp_array = (seq_t *)calloc(total, sizeof(seq_t));
#endif

    /* base: dp[0][0] = 1 */
    _dp_array[0] = (seq_t)1;

    /* DP over all possible part sizes */
    for (size_t val = 1; val <= Nmax; ++val) {
        for (size_t sum = Nmax - val; ; --sum) {
            for (size_t k = _dp_kmax - 1; ; --k) {
                seq_t addend = _dp_array[sum * cols + k];
                if (addend) {
                    _dp_array[(sum + val) * cols + (k + 1)] += addend;
                }
                if (k == 0) break;
            }
            if (sum == 0) break;
        }
    }
    _dp_nmax = Nmax;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* offset 1: undefined for n < 1 */

    _build_dp(n);

    size_t cols = _dp_kmax + 1;
    seq_t result = (seq_t)0;

    /* iterate all divisors d of n */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* case d = i */
            idx_t d1 = i;
            idx_t m1 = n / d1;           /* m = n/d */
            if (d1 <= _dp_kmax) {
                result += _dp_array[(size_t)m1 * cols + (size_t)d1];
            }
            /* complementary divisor d = n/i */
            idx_t d2 = n / i;
            if (d2 != d1 && d2 <= _dp_kmax) {
                idx_t m2 = n / d2;       /* equals i */
                result += _dp_array[(size_t)m2 * cols + (size_t)d2];
            }
        }
    }
    return result;
}
