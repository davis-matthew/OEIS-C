/*
 * A321258  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = sigma_k(n) - n^k.
 * https://oeis.org/A321258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sigma_k(row) - row^k, where (row, k) is the (n‑th) entry
 *            read by antidiagonals of the array A(n,k) = sum_{d|n} d^k - n^k.
 *            The mapping from the index n to (row, k) is obtained by solving
 *            row + k = s and n = s(s–1)/2 + row
 * Range:     u8 n <= 60, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t sigma_k(idx_t m, uint8_t k)
{
    seq_t res = (seq_t)1;
    idx_t n = m;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }

            /* compute pk = p^k */
            seq_t pk = (seq_t)1;
            for (idx_t i = 0; i < k; ++i) pk *= (seq_t)p;

            /* sum_{i=0}^{exp} (p^k)^i = 1 + pk + pk^2 + ...   */
            seq_t term = (seq_t)0;
            seq_t pow   = (seq_t)1;
            for (idx_t i = 0; i <= exp; ++i) {
                term += pow;
                pow  *= pk;
            }
            res *= term;
        }
    }

    if (n > 1) {               /* remaining prime factor with exponent 1 */
        /* k is 1 or 3, exp = 1 */
        seq_t pk = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) pk *= (seq_t)n;

        seq_t term = (seq_t)0;
        seq_t pow   = (seq_t)1;
        for (idx_t i = 0; i <= (idx_t)1; ++i) {
            term += pow;
            pow  *= pk;
        }
        res *= term;
    }

    return res;
}

/* Find least s such that s*(s+1)/2 ≥ n */
static idx_t min_s(idx_t n)
{
    idx_t s = 1;
    /* Double until we exceed n */
    while (( (__int128)s * (s + 1) ) / 2 < n)
        s <<= 1;

    idx_t lo = 0, hi = s;
    while (lo + 1 < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if (( (__int128)mid * (mid + 1) ) / 2 >= n)
            hi = mid;
        else
            lo = mid;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Input is assumed to be n ≥ 1. */
    if (n < 1) return 0;

    /* Determine the antidiagonal number s. */
    idx_t s = min_s(n);

    /* Number of terms before diagonal s. */
    idx_t pre = (idx_t)(( (__int128)(s - 1) * s ) / 2);

    /* Position within diagonal s (0‑based). */
    idx_t pos = n - pre - 1;

    /* Map to row and column. */
    idx_t row = pos + 1;
    idx_t k   = s - row;   /* k ≥ 0 */

    /* Compute sigma_k(row). */
    seq_t sigma = sigma_k(row, (uint8_t)k);

    /* Compute row^k explicitly (k ≤ ~140 for the problem range). */
    seq_t power = 1;
    for (idx_t i = 0; i < k; ++i)
        power *= (seq_t)row;

    /* Return the difference. */
    return sigma - power;
}
