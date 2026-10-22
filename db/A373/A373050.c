/*
 * A373050  Triangle read by rows: T(n, k) = (Sum_{i=0..n-k} (-1)^i *
 *          binomial(n-k, i) * (n+2-i)!) * binomial(n, k) / ((k+1) * (k+2))
 *          for 0 <= k <= n.
 * https://oeis.org/A373050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   my_fact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A373050: Triangle read by rows
   T(n, k) = (Σ_{i=0}^{n-k} (-1)^i · binomial(n-k, i) · (n+2-i)!)
            · binomial(n, k) / ((k+1)(k+2))
   a(n) is the n‑th element of this triangle read row‑wise (0‑based). */

/*  Note: no external games are used – every helper is implemented
    locally with only built‑in integer arithmetic.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/my_fact.h"

/* ---------------------------- local utilities ---------------------------- */

/* multiplicative binomial coefficient: C(n, k) */
static inline seq_t my_binom(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;          /* exact division */
    }
    return res;
}

/* find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2  */
static inline idx_t find_row(idx_t n) {
    idx_t lo = 0, hi = n;
    idx_t r = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n) {
            r = mid;
            lo = mid + 1;
        } else {
            (hi = mid - 1);
        }
    }
    return r;
}

/* --------------------------- main implementation ------------------------ */

seq_t a(idx_t n) {
    /* Handle the very first element trivially */
    if (n == 0) return (seq_t)1;

    /* Determine (row r, column k) of the n‑th element */
    idx_t r = find_row(n);
    idx_t first_in_row = r * (r + 1) / 2;
    idx_t k = n - first_in_row;

    /* Compute the summation part S = Σ (-1)^i * C(r-k,i) * (r+2-i)!   */
    idx_t max_i = r - k;                /* upper limit of the sum */
    seq_t fact = my_fact(r + 2);        /* start with (r+2)!   */
    seq_t S = (seq_t)0;
    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t term = my_binom(max_i, i) * fact;
        if ((i & 1U) == 0) S += term;
        else             S -= term;

        /* prepare fact for next i: divide by (r+2-i) */
        if (i < max_i) {
            fact /= (seq_t)(r + 2 - i);
        }
    }

    /* Final formula: result = S * C(r,k) / ((k+1)(k+2)) */
    seq_t binoc = my_binom(r, k);
    seq_t denom = (seq_t)(k + 1) * (seq_t)(k + 2);
    seq_t result = (S * binoc) / denom;

    return result;
}
