/*
 * A374451  Triangle T(n, k), n > 1, k = 1..n-1, read by rows; T(n, k) is the
 *          least prime number p such that the p-adic valuations of n and k
 *          differ.
 * https://oeis.org/A374451
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 92 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Implements OEIS A374451: For index n (n ≥ 2) flatten the triangle
 * T(row,k) by rows where row > 1 and 1 ≤ k < row.
 * T(row,k) is the least prime p such that the p‑adic valuations of
 * row and k differ.
 *
 * The mapping from the flat index n to (row,k):
 *   i  = n − 2          // zero‑based flat position
 *   Let m = ⌊(√(8*i+1)−1)/2⌋
 *   row = m + 2
 *   col = i − m(m+1)/2 + 1
 *
 * T(row,col) is computed by factorising row and col and returning the
 * smallest prime where their exponents differ.
 */
#include <stdint.h>
#include <stddef.h>

/* Simple trial‑division factorisation.  Stores primes and exponents
 * in the provided arrays.  Arrays must be large enough (here 64 works).
 */
static void factor_u64_my(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    *cnt = 0;
    if (n < 2)
        return;
    uint64_t m = n;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            pr[*cnt] = p;
            ex[*cnt] = e;
            ++(*cnt);
        }
    }
    if (m > 1) {
        pr[*cnt] = m;
        ex[*cnt] = 1;
        ++(*cnt);
    }
}

/* Integer square root of a 128‑bit unsigned integer. */
static uint64_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1) << 64;
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (uint64_t)(lo - 1);
}

seq_t a(idx_t n)
{
    /* Convert flat index to row and column.          */
    uint64_t i = (uint64_t)n - 2u;                      /* zero‑based index */
    unsigned __int128 temp = (unsigned __int128)8 * i + 1;
    uint64_t m = (isqrt_u128(temp) - 1) / 2;            /* m = row – 2   */
    uint64_t row = m + 2;                               /* actual row    */
    unsigned __int128 prev = (unsigned __int128)m * (m + 1) / 2;   /* T_m */
    uint64_t col = (uint64_t)((i - prev) + 1);          /* column (1‑based) */

    /* Factor both numbers.  64 slots are plenty for any row < 2^64. */
    uint64_t pr_row[64], ex_row[64], pr_col[64], ex_col[64];
    size_t cnt_row, cnt_col;
    factor_u64_my(row, pr_row, ex_row, &cnt_row);
    factor_u64_my(col, pr_col, ex_col, &cnt_col);

    /* Merge the two sorted prime lists to find the least prime with
     * differing exponents.                                     */
    size_t i_row = 0, i_col = 0;
    while (i_row < cnt_row && i_col < cnt_col) {
        uint64_t p_row = pr_row[i_row];
        uint64_t p_col = pr_col[i_col];
        if (p_row == p_col) {
            if (ex_row[i_row] != ex_col[i_col])
                return (seq_t)p_row;
            ++i_row; ++i_col;
        } else if (p_row < p_col) {
            return (seq_t)p_row;      /* exponent in col is 0   */
        } else { /* p_col < p_row */
            return (seq_t)p_col;
        }
    }
    /* Remaining primes (if any) belong only to one number. */
    if (i_row < cnt_row)
        return (seq_t)pr_row[i_row];
    return (seq_t)pr_col[i_col];   /* i_col < cnt_col guaranteed here */
}
