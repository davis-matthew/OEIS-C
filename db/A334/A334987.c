/*
 * A334987  Sum of centered triangular numbers dividing n.
 * https://oeis.org/A334987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of centered triangular numbers dividing n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function sums all divisors d of n such that d is of that form. */
static inline int is_centered_triangular(idx_t d)
{
    if (d == 0)
        return 0;
    idx_t m = 2 * (d - 1);
    if (m % 3 != 0)
        return 0;
    idx_t m2 = m / 3;
    idx_t s = 1 + 4 * m2;

    /* integer square root search (no floating‑point operations) */
    idx_t low = 0, high = s;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq = mid * mid;
        if (sq == s) {
            /* k = (1 + mid) / 2 must be integer, i.e. 1 + mid even */
            return ((1 + mid) & 1) ? 0 : 1;
        }
        if (sq < s)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return 0;
}
/* Centered triangular numbers are given by C_k = 3*k*(k - 1)/2 + 1. */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if (is_centered_triangular(d1))
                sum += (seq_t)d1;
            if (d2 != d1 && is_centered_triangular(d2))
                sum += (seq_t)d2;
        }
    }
    return sum;
}
