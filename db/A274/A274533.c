/*
 * A274533  Irregular triangle read by rows: T(n,k) = sum of the elements of
 *          the k-th column of the absolute difference table of the divisors
 *          of n.
 * https://oeis.org/A274533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A274533: a(n) is the n‑th entry of the triangle
   T(r,k) where T(r,k) is the sum of the k‑th column of the
   absolute‑difference table of the divisors of r,
   read row by row. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Helper to count divisors of x (x ≥ 1). */
static idx_t divisor_count(idx_t x)
{
    idx_t c = 0;
    for (idx_t i = 1; i <= x / i; ++i)
    {
        if (x % i == 0)
        {
            ++c;
            if (i != x / i)
                ++c;
        }
    }
    return c;
}

/* Build a sorted list of the divisors of r in the array *divs.
   The array must be large enough to hold all divisors.
   Returns the number of divisors. */
static idx_t divisors(idx_t r, idx_t *divs)
{
    idx_t small[256], large[256];
    idx_t ns = 0, nl = 0;
    for (idx_t i = 1; i <= r / i; ++i)
    {
        if (r % i == 0)
        {
            small[ns++] = i;
            if (i != r / i)
                large[nl++] = r / i;
        }
    }
    idx_t total = ns + nl;
    for (idx_t i = 0; i < ns; ++i)
        divs[i] = small[i];
    for (idx_t i = 0; i < nl; ++i)
        divs[ns + i] = large[nl - 1 - i];
    return total;
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    /* ----------------------------------------------------------
       1. Locate the row r and column k (0‑based) that the index
          n corresponds to.  We keep a running prefix sum of
          divisor counts.
       ---------------------------------------------------------- */
    static idx_t *pref = NULL;       /* pref[0] = 0; pref[1] = d(1); ... */
    static size_t pref_cap = 0;
    static size_t pref_len = 0;      /* number of rows currently stored */

    if (pref == NULL)
    {
        pref_cap = 128;
        pref_len  = 1;
        pref = (idx_t *)malloc(pref_cap * sizeof(idx_t));
        pref[0] = 0;
    }

    /* Extend pref until it covers n. */
    while (pref[pref_len - 1] < n)
    {
        idx_t r = pref_len;           /* next row number */
        if (pref_len >= pref_cap)
        {
            pref_cap <<= 1;
            pref = (idx_t *)realloc(pref, pref_cap * sizeof(idx_t));
        }
        pref[pref_len] = pref[pref_len - 1] + divisor_count(r);
        ++pref_len;
    }

    /* Binary search for smallest r with pref[r] >= n. */
    idx_t lo = 1, hi = (idx_t)(pref_len - 1), r = 1;
    while (lo <= hi)
    {
        idx_t mid = (lo + hi) >> 1;
        if (pref[mid] >= n)
        {
            r = mid;
            if (mid == 0) break;
            hi = mid - 1;
        }
        else
            lo = mid + 1;
    }

    /* Column index within the row (0‑based). */
    idx_t col = n - pref[r - 1] - 1;

    /* ----------------------------------------------------------
       2. Compute T(r,k) for the requested row r.
       ---------------------------------------------------------- */
    idx_t div_buf[256];
    idx_t d = divisors(r, div_buf);

    /* Allocate arrays for current and next rows of the difference
       table, and for the accumulated column sums. */
    seq_t *cur  = (seq_t *)malloc(d * sizeof(seq_t));
    seq_t *sums = (seq_t *)calloc(d, sizeof(seq_t));

    /* Initialize current row with the divisors (as seq_t). */
    for (idx_t i = 0; i < d; ++i)
        cur[i] = (seq_t)div_buf[i];

    idx_t len = d;
    while (len > 0)
    {
        /* Add current row to cumulative sums. */
        for (idx_t j = 0; j < len; ++j)
            sums[j] += cur[j];

        if (len == 1) break;

        /* Build the next row of absolute differences. */
        seq_t *next = (seq_t *)malloc((len - 1) * sizeof(seq_t));
        for (idx_t j = 0; j < len - 1; ++j)
        {
            seq_t a = cur[j];
            seq_t b = cur[j + 1];
            next[j] = (a > b) ? (a - b) : (b - a);
        }

        free(cur);
        cur = next;
        len--;
    }

    seq_t result = sums[col];

    /* Clean up. */
    free(cur);
    free(sums);

    return result;
}
