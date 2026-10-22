/*
 * A338655  a(n) = Sum_{d|n} phi(d) * binomial(d+n/d-1, d).
 * https://oeis.org/A338655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d) * binomial(d + n/d - 1, d)
 * Range:     u8 n <= 17, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* --- integer square root (floor) for idx_t --------------------------------- */
static idx_t int_sqrt_idx(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  low  = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

/* --- Euler's totient function for an idx_t, returning seq_t ****************/
static seq_t phi_idx(idx_t n)
{
    seq_t result = n;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            result = result / p * (p - 1);
        }
    }
    if (t > 1) {
        result = result / t * (t - 1);
    }
    return result;
}

/* --- Binomial coefficient C(n,k) for idx_t arguments, returns seq_t ------ */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* --- Main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of range – harmless value */

    seq_t total = 0;
    idx_t r = int_sqrt_idx(n);

    for (idx_t d = 1; d <= r; ++d) {
        if (n % d != 0) continue;

        /* divisor d */
        idx_t d1 = d;
        seq_t phi1 = phi_idx(d1);
        idx_t t1   = d1 + n / d1 - 1;
        seq_t bin1 = binom_idx(t1, d1);
        total = total + phi1 * bin1;

        /* corresponding divisor n/d if distinct */
        idx_t d2 = n / d;
        if (d2 != d1) {
            seq_t phi2 = phi_idx(d2);
            idx_t t2   = d2 + n / d2 - 1;   /* n / d2 == d1 */
            seq_t bin2 = binom_idx(t2, d2);
            total = total + phi2 * bin2;
        }
    }

    return total;
}
