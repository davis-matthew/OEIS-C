/*
 * A395065  Triangle read by rows: coefficients of
 *          (1+(2*n-1)*x)*(1+(n-1)*x)^(n-1).
 * https://oeis.org/A395065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = coefficient of x^k in (1+(2*n-1)x)*(1+(n-1)x)^(n-1),
 *            flattened by rows (row 0 is 1)
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t binomial_multiplicative(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Handle row 0 (n = 0) separately. */
    if (n == 0) return 1;

    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;                   /* current row (0-indexed) */
    idx_t m = r - 1;                  /* parameter m = n-1 of the definition */
    idx_t start = (unsigned __int128)r * (r + 1) / 2; /* start index of row r */
    idx_t k = n - start;               /* position within the row */

    /* Base for powers: r-1 = m. */
    seq_t base = (seq_t)m;

    /* term1 = C(m, k) * m^k   (k may be 0..m+1). */
    seq_t term1 = 0;
    if (k <= m) {
        seq_t bin = binomial_multiplicative(m, k);
        seq_t pw   = pow_seq(base, k);
        term1 = bin * pw;
    }

    /* term2 = (2*r-1) * C(m, k-1) * m^(k-1), only for k >= 1. */
    seq_t term2 = 0;
    if (k >= 1) {
        seq_t bin = binomial_multiplicative(m, k - 1);
        seq_t pw   = pow_seq(base, k - 1);
        term2 = bin * pw * (seq_t)(2 * r - 1);
    }

    return term1 + term2;
}
