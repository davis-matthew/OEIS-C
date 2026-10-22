/*
 * A362175  Least number k > 1 not a power of 10 such that k^n, n > 2, starts
 *          with k, or -1 if no such number exists.
 * https://oeis.org/A362175
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k > 1, not a power of 10, such that k^n (n > 2)
 *            starts with the decimal digits of k; return -1 if no such k
 *            exists
 * Range:     u8 n <= 3, u16 n <= 104, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

/* helper: true if k is an exact power of 10 (k = 10^m, m ≥ 1) */
static bool is_power_of_10(idx_t k) {
    if (k < 10) return false;
    while (k % 10 == 0) k /= 10;
    return k == 1;
}

/* helper: number of decimal digits of k (k > 0) */
static unsigned int digit_count_idx(idx_t k) {
    unsigned int d = 0;
    do {
        ++d;
        k /= 10;
    } while (k);
    return d;
}

/* helper: does k^n begin with the decimal representation of k?  */
static bool leading_equal(idx_t k, idx_t n) {
    unsigned int d = digit_count_idx(k);
    long double logk = log10l((long double)k);
    long double x = (long double)n * logk;
    long double intpart;
    long double frac = modfl(x, &intpart);
    long double leading = powl(10.0L, frac + (long double)(d - 1));
    /* add tiny epsilon to guard against rounding errors */
    uint64_t leading_int = (uint64_t)(leading + 1e-12L);
    return leading_int == k;
}

/* the requested function */
seq_t a(idx_t n) {
    if (n < 3) return (seq_t)-1;   /* not defined below offset */

    for (idx_t k = 2; ; ++k) {
        if (is_power_of_10(k) || k == 1) continue;
        if (leading_equal(k, n)) {
            return (seq_t)k;
        }
    }
}
