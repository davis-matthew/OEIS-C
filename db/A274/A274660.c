/*
 * A274660  Irregular triangle read by rows in which row n lists the divisors
 *          d of 2*n+1 (A274658), given the sign (-1)^(n + (d-1)/2).
 * https://oeis.org/A274660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   a(n) is the nth term of the irregular triangle read by rows. For
 *            a row index r = ⌊?⌋, let m = 2*r+1. Let d_k be the k‑th divisor
 *            of m (divisors in increasing order). Then a(n) = (-1)^(r +
 *            (d_k-1)/2) * d_k, where k is the position of n inside row r. Row
 *            r contains τ(2*r+1) entries (τ = number of d...
 * Range:     i8 n <= 205, i16 n <= 12574, i32 n <= 12574, i64 n <= 12574,
 *            i128 n <= 12574, big any n
 * Validated: standard, pass, 102 terms through n = 8731 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: count divisors of m */
static idx_t divisor_count_ll(uint64_t m)
{
    idx_t cnt = 0;
    for (idx_t d = 1; d <= m / d; ++d)
    {
        if (m % d == 0)
        {
            ++cnt;
            if (d * d != m)
                ++cnt;
        }
    }
    return cnt;
}

/* Helper: return k-th divisor (0‑based) of m, divisors sorted ascending */
static uint64_t divisor_k_ll(uint64_t m, idx_t k)
{
    /* maximum number of divisors for m <= 2*10^7 is < 256, safe for static array */
    seq_t divs[256];
    size_t num = 0;
    for (uint64_t d = 1; d <= m / d; ++d)
    {
        if (m % d == 0)
        {
            divs[num++] = (seq_t)d;
            if (d * d != m)
                divs[num++] = (seq_t)(m / d);
        }
    }
    /* simple insertion sort */
    for (size_t i = 0; i < num; ++i)
        for (size_t j = i + 1; j < num; ++j)
            if (divs[j] < divs[i])
            {
                seq_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }
    return (uint64_t)divs[k];
}

/* Static storage for cumulative divisor counts: cum[i] = Σ_{t=0..i} τ(2*t+1) */
static idx_t *cum = NULL;
static size_t cum_cap = 0;
static size_t cum_size = 0;

/* Increase cum until it exceeds n */
static void ensure_cumulative(idx_t n)
{
    while (cum_size == 0 || cum[cum_size - 1] <= n)
    {
        idx_t r = (idx_t)cum_size;            /* next row index */
        uint64_t m = 2 * (uint64_t)r + 1;      /* 2*r+1 */
        idx_t cnt = divisor_count_ll(m);

        if (cum_size + 1 > cum_cap)
        {
            size_t new_cap = cum_cap ? cum_cap * 2 : 16;
            cum = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
            cum_cap = new_cap;
        }
        if (cum_size == 0)
            cum[0] = cnt;
        else
            cum[(size_t)cum_size] = cum[(size_t)cum_size - 1] + cnt;
        ++cum_size;
    }
}

/* Find row r such that cum[r-1] <= n < cum[r] */
static idx_t find_row(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)cum_size - 1;
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum[mid] > n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Build cumulative table up to n */
    ensure_cumulative(n);

    /* Locate row and position within the row */
    idx_t r = find_row(n);
    idx_t prev = (r == 0) ? 0 : cum[r - 1];
    idx_t k = n - prev;                 /* 0‑based index in the row */

    uint64_t m = 2ULL * (uint64_t)r + 1ULL;
    uint64_t d = divisor_k_ll(m, k);

    /* Compute the sign (-1)^(r + (d-1)/2) */
    int sign = ((r + ((d - 1) / 2)) & 1) ? -1 : 1;

    return (seq_t)(sign * (seq_t)d);
}
