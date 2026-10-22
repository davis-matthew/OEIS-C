/*
 * A350770  Triangle read by rows: T(n, k) = 2^(n-k-1) + 2^k - 2, 0 <= k <=
 *          n-1.
 * https://oeis.org/A350770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   * a(n) is the nth entry of the triangle read by rows: * T(r , k)
 *            = 2^(r-k-1) + 2^k - 2, * where r is the unique integer with
 *            (r-1)*r/2 < n ≤ r*(r+1)/2 * and k = n - (r-1)*r/2 - 1
 * Range:     u8 n <= 45, u16 n <= 153, u32 n <= 561, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* fast binary‑exponentiation for unsigned power of two without UB from shifts */
static seq_t pow2_exp(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;

    while (exp)
    {
        if (exp & 1ULL)
            result *= base;      /* unsigned overflow is defined modulo */
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* find the row r that contains the nth element */
    /* r ≈ (sqrt(8n+1)-1)/2; adjust if necessary */
    long double d = sqrt((long double)(8.0L * (long double)n + 1.0L));
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* correct potential off‑by‑one due to round‑off */
    while ((r * (r + 1) / 2) < n) ++r;
    while (r > 1 && ((r - 1) * r / 2) >= n) --r;

    /* compute k (zero‑based column index) */
    idx_t prev_rows = (r - 1) * r / 2;
    idx_t k = n - prev_rows - 1;   /* 0 ≤ k ≤ r-1 */

    /* T(r,k) = 2^(r-k-1) + 2^k - 2 */
    seq_t term1 = pow2_exp(r - k - 1);
    seq_t term2 = pow2_exp(k);
    seq_t result = term1 + term2;
    result -= (seq_t)2;

    return result;
}
