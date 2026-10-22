/*
 * A353602  Square array read by downward antidiagonals: A(n, k) = k-th
 *          Wieferich base of n, i.e., k-th b > 1 such that b^(n-1) == 1 (mod
 *          n^2).
 * https://oeis.org/A353602
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = antidiagonal traversal of Wieferich bases A(n,k)
 * Range:     u8 n <= 52, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    /* offset is 2; a(2) is the first term */
    if (n < 2) return 0;

    /* k is the 1‑based position of the term in the cumulative list */
    idx_t k = n - 1;

    /* Find t such that t(t+1)/2 >= k   (t = number of terms on the current diagonal) */
    idx_t t = (idx_t)((sqrt(8.0 * (double)k + 1.0) - 1.0) / 2.0);
    /* adjust for rounding errors */
    while ((unsigned __int128)t * (t + 1) / 2 < k) t++;
    while (t > 0 && (unsigned __int128)(t - 1) * t / 2 >= k) t--;

    /* Position inside diagonal (1‑based) */
    idx_t before = (unsigned __int128)(t - 1) * t / 2;
    idx_t pos = k - before;          /* 1 <= pos <= t */

    /* n (row) and k (column) of the desired cell in the array A(n,k) */
    idx_t row = pos + 1;             /* i */
    idx_t col = t + 1 - pos;         /* j */

    /* Find the col‑th Wieferich base of row */
    seq_t base = 2;
    idx_t count = 0;
    seq_t mod = (seq_t)row * (seq_t)row;    /* n^2 */

    while (true) {
        if (mod_pow(base, row - 1, mod) == 1) {
            count++;
            if (count == col) return base;
        }
        base++;
    }
}
