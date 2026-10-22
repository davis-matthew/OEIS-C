/*
 * A336746  Triangle read by rows: T(n,k) = (n-k-1+H(k+1))*((k+1)!) for 0 <= k
 *          <= n where H(k+1) = Sum_{i=0..k} 1/(i+1) for k >= 0.
 * https://oeis.org/A336746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   triangle read by rows * T(i,0) = i * T(i,k) = k! + (k+1) *
 *            T(i-1,k-1) for 1 ≤ k ≤ i * a(n) = T(r,k) where r*(r+1)/2 ≤ n <
 *            (r+1)*(r+2)/2 and k = n - r*(r+1)/2 * * The implementation
 *            builds the triangle row‑by‑row up to the needed * row r, using a
 *            dynamically resized factorial table
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- factorial table ---------------------------------------- */
static seq_t *fact = NULL;          /* factorial[0..] : fact[i] = i!      */
static size_t fact_cap = 0;         /* number of factorials currently stored */

/* ensure that fact[0..max] are available */
static void ensure_factorial(size_t max)
{
    if (max + 1 > fact_cap) {
        size_t new_cap = max + 1;
        seq_t *new_fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!new_fact) abort();
        if (fact_cap == 0) {
            new_fact[0] = (seq_t)1;   /* 0! = 1 */
            fact_cap = 1;
        }
        for (size_t i = fact_cap; i < new_cap; ++i)
            new_fact[i] = new_fact[i-1] * (seq_t)i;  /* i! = (i-1)! * i */
        fact = new_fact;
        fact_cap = new_cap;
    }
}

/* ---------- locate the row containing the n‑th term --------------- */
static inline idx_t find_row(idx_t n)
{
    /* Find the largest r >= 0 with r*(r+1)/2 <= n.
     * A doubling search provides an upper bound for the binary search.
     */
    idx_t low = 0;
    idx_t high = 1;
    while ((__uint128_t)high * (high + 1) / 2 <= n) {
        low = high;
        high <<= 1;
        if (high == 0) {          /* overflow: set to maximum value */
            high = (idx_t)(~(idx_t)0);
            break;
        }
    }

    /* Binary search between low and high */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__uint128_t)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* ---------- main sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* a(0) = 0 */

    /* Determine target row r and column k */
    idx_t r = find_row(n);
    idx_t row_start = (idx_t)((__uint128_t)r * (r + 1) / 2);
    idx_t k = n - row_start;

    /* Bound: maximum k is r */
    ensure_factorial((size_t)r);   /* need factorials up to r! */

    /* Allocate two rows of length r+1 (max needed) */
    size_t sz = (size_t)r + 1;
    seq_t *rowA = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *rowB = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!rowA || !rowB) {
        if (rowA) free(rowA);
        if (rowB) free(rowB);
        abort();
    }

    /* Row 0 */
    rowA[0] = (seq_t)0;  /* T(0,0)=0 */
    seq_t *prev = rowA;
    seq_t *curr = rowB;

    /* Build rows 1 .. r */
    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = (seq_t)i;                     /* T(i,0) = i */
        for (idx_t kk = 1; kk <= i; ++kk) {
            curr[kk] = fact[kk] + (seq_t)(kk + 1) * prev[kk - 1];
        }
        /* Swap rows for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[(size_t)k];   /* prev now holds row r */

    free(rowA);
    free(rowB);
    return result;
}
