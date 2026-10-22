/*
 * A370698  Rectangular array, read by antidiagonals: row n consists of the
 *          numbers m whose binary representation has exactly n runs.
 * https://oeis.org/A370698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Unranking algorithm for OEIS A370698:
   numbers read by antidiagonals of the array "row n = numbers whose binary
   representation has exactly n runs".  The r‑th row is sorted
   in increasing numeric order.  The sequence a(n) is the n‑th element
   of this 2‑D array read by antidiagonals.  */
#include <stdint.h>
#include "../utils/pow2.h"

/* n, k, cap are unsigned 64‑bit integers; the calculation is done in
   unsigned __int128 to avoid overflow.  Return cap+1 if the true binomial
   coefficient exceeds cap. */
static inline idx_t binom_capped(idx_t n, idx_t k, idx_t cap)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res /= i;
        if (res > (__int128)cap + 1) return cap + 1;
    }
    return (idx_t)res;
}

/* Find the diagonal number k (1‑based) such that
   (k-1)(k-2)/2 < n ≤ (k-1)k/2.  */
static idx_t find_diagonal(idx_t n)
{
    idx_t low = 2, high = 2;
    while ((__int128)(high - 1) * high / 2 < n) {
        high <<= 1;
        if (high == 0) break;          /* overflow guard */
    }
    low = 2;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)(mid - 1) * mid / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Unrank the run‑length vector for row r, total length L, and rank (0‑based)
   within that length.  The ordering is:
   odd runs (1st, 3rd, …) ascending, even runs descending. */
static void compute_runs(idx_t r, idx_t L, idx_t rank, idx_t *runs)
{
    idx_t remRuns = r;
    idx_t remSum  = L;
    idx_t pos = 1;                 /* 1‑based position in the run vector */

    while (remRuns > 1) {
        idx_t minVal = 1;
        idx_t maxVal = remSum - (remRuns - 1);
        if (pos & 1) {            /* odd run: ascending order */
            for (idx_t val = minVal; val <= maxVal; ++val) {
                idx_t cnt =
                    binom_capped(remSum - val - 1, remRuns - 2, rank + 1);
                if (rank < cnt) {
                    runs[pos - 1] = val;
                    remSum -= val;
                    remRuns--;
                    pos++;
                    break;
                } else {
                    rank -= cnt;
                }
            }
        } else {                  /* even run: descending order */
            for (idx_t val = maxVal; val >= minVal; --val) {
                idx_t cnt =
                    binom_capped(remSum - val - 1, remRuns - 2, rank + 1);
                if (rank < cnt) {
                    runs[pos - 1] = val;
                    remSum -= val;
                    remRuns--;
                    pos++;
                    break;
                } else {
                    rank -= cnt;
                }
            }
        }
    }
    /* last run */
    runs[r - 1] = remSum;
}

/* Convert the run lengths to the binary integer value. */
static seq_t value_from_runs(const idx_t *runs, idx_t r)
{
    seq_t val = 0;
    int bit_type = 1;               /* 1 → run of 1's, 0 → run of 0's */
    for (idx_t i = 0; i < r; ++i) {
        idx_t len = runs[i];
        val <<= len;
        if (bit_type) {
            val |= pow2(len) - 1;   /* set the lower 'len' bits to 1 */
        }
        bit_type ^= 1;
    }
    return val;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n=0, harmless */

    /* Locate the diagonal (k) and the position within that diagonal. */
    idx_t k = find_diagonal(n);
    idx_t prev = (k - 1) * (k - 2) / 2;   /* elements before this diagonal */
    idx_t inside = n - prev;              /* 1‑based index inside diagonal */
    idx_t r = inside;                     /* number of runs (row index) */
    idx_t c = k - r;                      /* column index within the row */

    /* Rank (0‑based) within the entire row r. */
    idx_t rank = c - 1;

    /* Determine total length L for the desired element. */
    idx_t L = r;
    while (1) {
        idx_t cnt = binom_capped(L - 1, r - 1, rank + 1);
        if (rank < cnt) break;
        rank -= cnt;
        ++L;
    }

    /* Unrank the run lengths. */
    idx_t runs[200];   /* r ≤ 141 for n ≤ 10000, buffer is safe */
    compute_runs(r, L, rank, runs);

    /* Build and return the integer value. */
    return value_from_runs(runs, r);
}
