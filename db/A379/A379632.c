/*
 * A379632  Irregular triangle read by rows in which row n lists the smallest
 *          parts of the partitions of n into consecutive parts (with the
 *          partitions ordered by increasing number of parts).
 * https://oeis.org/A379632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the nth term of OEIS A379632. The sequence is obtained by
 *            listing, row‑by‑row, the smallest part of each representation of
 *            the row index n as a sum of consecutive positive integers,
 *            ordered by the number of parts. There is no simple closed form,
 *            so the value is computed by enumerating all...
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, mid;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

static idx_t *pref = NULL;          /* pref[i] = total terms up to row i, incl. zeros[0] */
static size_t pref_cap = 0;
static idx_t  last_row = 0;          /* largest row that has been processed */

/* ensure pref is computed up to a row whose cumulative terms ≥ n */
static void ensure_pref(idx_t n)
{
    if (!pref) {
        pref_cap = 16;
        pref = (idx_t *)malloc(pref_cap * sizeof(idx_t));
        if (!pref) exit(EXIT_FAILURE);
        pref[0] = 0;
        last_row = 0;
    }
    while (pref[last_row] < n) {
        ++last_row;
        idx_t r = last_row;

        idx_t count = 0;
        idx_t Lmax = (isqrt_u64((__uint128_t)8 * r + 1) - 1) / 2;

        for (idx_t L = 1; L <= Lmax; ++L) {
            idx_t t  = L * (L - 1) / 2;
            idx_t rem = r - t;
            if (rem % L == 0)
                ++count;
        }

        if (pref_cap <= last_row) {
            size_t new_cap = pref_cap ? pref_cap * 2 : 16;
            while (new_cap <= last_row) new_cap *= 2;
            idx_t *tmp = (idx_t *)realloc(pref, new_cap * sizeof(idx_t));
            if (!tmp) exit(EXIT_FAILURE);
            pref = tmp;
            pref_cap = new_cap;
        }
        pref[last_row] = pref[last_row - 1] + count;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined below offset */

    ensure_pref(n);

    /* locate the row r that contains the n‑th term */
    idx_t low = 1, high = last_row;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (pref[mid] < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t prev = pref[r - 1];
    idx_t pos   = n - prev - 1;     /* 0‑based position inside the row */

    /* find the pos‑th valid partition length L */
    idx_t Lmax = (isqrt_u64((__uint128_t)8 * r + 1) - 1) / 2;
    idx_t index = 0;
    seq_t result = 0;

    for (idx_t L = 1; L <= Lmax; ++L) {
        idx_t t   = L * (L - 1) / 2;
        idx_t rem = r - t;
        if (rem % L == 0) {
            if (index == pos) {
                result = (seq_t)(rem / L);
                break;
            }
            ++index;
        }
    }
    return result;
}
