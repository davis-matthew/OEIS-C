/*
 * A346533  Irregular triangle read by rows in which row n lists the first n -
 *          2 terms of A000203 together with the sum of A000203(n-1) and
 *          A000203(n), with a(1) = 1.
 * https://oeis.org/A346533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 2556, u16 n <= 9930, u32 n <= 9930, u64 n <= 9930,
 *            u128 n <= 9930, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A346533: irregular triangle read by rows.
 *  a(1) = 1.
 *  For n > 1:
 *   • Find the row r such that  r*(r-1)/2 ≥ n‑1  (the minimal r).
 *   • Let offset = n – (1 + (r‑2)(r‑1)/2) – 1   (0‑based position in row r).
 *   • If offset < r‑2  →  a(n) = σ(offset+1), where σ is the sum of divisors.
 *   • Otherwise (last element of the row)  →  a(n) = σ(r‑1) + σ(r).
 *
 *  The function below implements this rule exactly.
 */

#include <stdint.h>

/* helper: sum of divisors σ(n) for n ≥ 1 */
static seq_t sigma(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            seq_t add1 = (seq_t)d;
            seq_t add2 = (seq_t)(n / d);
            sum += add1;
            if (add1 != add2)
                sum += add2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;                    /* a(1) = 1 */

    /* find row r: minimal r with r*(r-1)/2 >= n-1 */
    idx_t k = n - 1;                  /* offset from first row after a(1) */
    idx_t low = 1, high = 2;
    /* expand high until condition holds (use 128‑bit to avoid overflow) */
    while ((unsigned __int128)high * (high - 1) / 2 < k)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * (mid - 1) / 2 >= k)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                    /* row number */

    /* cumulative count up to previous row */
    idx_t prev_count = 1 + (unsigned __int128)(r - 2) * (r - 1) / 2;
    idx_t offset = n - prev_count - 1;   /* 0‑based index in the row */

    if (offset < r - 2)           /* ordinary σ entry */
        return sigma(offset + 1);
    else                          /* last entry of the row: σ(r-1)+σ(r) */
        return sigma(r - 1) + sigma(r);
}
