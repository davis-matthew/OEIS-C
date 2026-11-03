/*
 * A391996  Table read by rows: T(n, k) = floor((k/2)^2)*n^2.
 * https://oeis.org/A391996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(i) = ⌊((k/2)²)⌋ · n², where n is the row index and k = i –
 *            n(n+1)/2 is the column offset obtained by counting terms
 *            row‑by‑row
 * Range:     u8 n <= 26, u16 n <= 298, u32 n <= 11475, u64 n <= 11475,
 *            u128 n <= 11475, big any n
 * Validated: standard, pass, 82 terms through n = 9555 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the largest r such that r(r+1)/2 <= idx. */
static idx_t _find_row(idx_t idx)
{
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= idx)
        high <<= 1;
    idx_t low = high >> 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= idx)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;               /* largest r with r(r+1)/2 <= idx */
}

seq_t a(idx_t idx)
{
    /* Determine row r and column offset k for linear index idx. */
    idx_t r = _find_row(idx);
    unsigned __int128 tri_r128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t tri_r = (idx_t)tri_r128;
    idx_t k = idx - tri_r;        /* k in [0, r] */

    /* Compute ⌊(k² / 4)⌋ * r² using seq_t arithmetic. */
    seq_t kk = (seq_t)k * (seq_t)k;     /* k² */
    seq_t part = kk / 4;                /* ⌊k²/4⌋ */
    seq_t rr = (seq_t)r * (seq_t)r;     /* r² */

    return part * rr;
}
