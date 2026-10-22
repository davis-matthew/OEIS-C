/*
 * A386492  Array read by ascending antidiagonals: A(n,m) = numerator(n*(m^2 -
 *          1)/(m^2 + 1)), where m > 0.
 * https://oeis.org/A386492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, tabl
 * Formula:   a(n) = numerator of n*(m^2 - 1)/(m^2 + 1) for (n,m) obtained by
 *            ascending antidiagonals (n >= 1, m > 0)
 * Range:     u8 n <= 72, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static inline idx_t diag_of(idx_t pos) {
    /* Find smallest s such that s*(s-1)/2 >= pos */
    idx_t low = 1, high = 2;
    while ((unsigned __int128)high * (high - 1) / 2 < pos) {
        high <<= 1;
    }
    low = high >> 1;
    if (low < 1) low = 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= pos)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* safety for non‑offset values */

    /* Locate the antidiagonal (s) and the position within it */
    idx_t s = diag_of(n);            /* diagonal number (s >= 2) */
    idx_t prev = s - 1;              /* previous diagonal count */
    unsigned __int128 cum_prev = (unsigned __int128)prev * (prev - 1) / 2;
    idx_t offset = n - (idx_t)cum_prev - 1;  /* 0‑based offset in this diagonal */
    idx_t n_val_idx = prev - offset;        /* row index (n) */
    seq_t n_val = (seq_t)n_val_idx;
    seq_t m_val = (seq_t)(s - n_val_idx);   /* column index (m) */

    seq_t m_sq = m_val * m_val;
    seq_t num_full = n_val * (m_sq - 1);    /* numerator before reduction */
    seq_t denom = m_sq + 1;                /* denominator */

    seq_t g = gcd(num_full, denom);        /* gcd of numerator and denominator */
    return num_full / g;                   /* reduced numerator */
}
