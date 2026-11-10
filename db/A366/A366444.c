/*
 * A366444  Triangle read by rows: T(n,k) = phi(n/k)*A023900(k) if k divides
 *          n, T(n,k) = 0 otherwise (n >= 1, 1 <= k <= n).
 * https://oeis.org/A366444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n,k) = phi(n/k)*A023900(k) if k divides n, else 0; a(n) is the
 *            triangle read by rows, A023900(k) = Product_{p|k} (1-p)
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static seq_t A366444_mu_prod(idx_t n)
{
    seq_t result = (seq_t)1;
    idx_t m = n;
    if (m > 1 && (m & 1u) == 0) {
        result = -result;
        while ((m & 1u) == 0) m >>= 1u;
    }
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            result *= ((seq_t)1 - (seq_t)p);
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) result *= ((seq_t)1 - (seq_t)m);
    return result;
}

static seq_t A366444_phi(idx_t n)
{
    idx_t m = n;
    idx_t res = n;
    for (idx_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            res -= res / p;
        }
    }
    if (m > 1) res -= res / m;
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    /* find row r with r(r-1)/2 < n <= r(r+1)/2 */
    idx_t r = (idx_t)((1.0 + sqrt(8.0 * (double)n)) / 2.0);
    if (r < 1) r = 1;
    while (r > 1 && r * (r - 1) / 2 >= n) r--;
    while (r * (r + 1) / 2 < n) r++;
    idx_t k = n - r * (r - 1) / 2;
    if (r % k != 0) return (seq_t)0;
    return A366444_phi(r / k) * A366444_mu_prod(k);
}
