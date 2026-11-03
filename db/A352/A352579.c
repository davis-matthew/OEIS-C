/*
 * A352579  Irregular triangle T(n,k) (n >= 1, 0 <= k <= 2^n-2) read by rows:
 *          row n = 1 is [0]; thereafter, T(n,k) = 2*T(n-1,k)+(k mod 2) and
 *          T(n,k+2^(n-1)-1) = 2*T(n-1,k)+(k+1 mod 2) for k = 0..2^(n-1)-2;
 *          T(n,2^n-2) = 2^n-2.
 * https://oeis.org/A352579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth element (starting at 1) of the irregular
 *            triangle T(n,k) read row by row. The triangle is defined by
 *            T(1,0)=0 and for n≥2, let s=2^(n-1)-1. For 0≤k<s: T(n,k) =
 *            2*T(n-1,k) + (k mod 2) For 0≤k<s: T(n,k+s) = 2*T(n-1,k) + ((k+1)
 *            mod 2) T(n,2^n-2) = 2^n-2
 * Range:     u8 n <= 504, u16 n <= 16369, u32 n <= 16369, u64 n <= 16369,
 *            u128 n <= 16369, big any n
 * Validated: standard, pass, 136 terms through n = 9962 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _T(idx_t row, idx_t k)
{
    if (row == 1)                     /* base row */
        return 0;

    /* last element of the row */
    if (k == ((idx_t)1 << row) - 2)
        return k;

    idx_t s = ((idx_t)1 << (row - 1)) - 1;

    if (k < s)                        /* first half */
        return 2 * _T(row - 1, k) + (seq_t)(k & 1);
    else                              /* second half */
    {
        idx_t kprime = k - s;         /* corresponding index in previous row */
        return 2 * _T(row - 1, kprime) + (seq_t)((kprime + 1) & 1);
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n<1, return 0 as a guard */

    /* find the row that contains position n */
    idx_t cumulative = 0;
    idx_t r = 0;
    for (;;)
    {
        ++r;
        idx_t len = ((idx_t)1 << r) - 1;     /* length of row r */
        cumulative += len;
        if (n <= cumulative)                /* n lies in this row */
        {
            idx_t prev_cumulative = cumulative - len;
            idx_t k = n - prev_cumulative - 1;   /* 0‑based column in row r */
            return _T(r, k);
        }
    }
}
