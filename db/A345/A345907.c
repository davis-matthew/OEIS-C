/*
 * A345907  Triangle giving the main antidiagonals of the matrices counting
 *          integer compositions by length and alternating sum (A345197).
 * https://oeis.org/A345907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th term (0‑based) of the triangle that lists the 
 * reversed main antidiagonal of A345197’s matrices M_m where m = r+1, 
 * with r = ⌊(√(1+8n)−1)/2⌋ the row index.
 * The value is computed as the number of compositions of m with length k 
 * and alternating sum i, where k = m – c, c = n – r(r+1)/2, and 
 * i = 2c – (r‑1).  This is calculated by
 *
 *   O = (i + m)/2  (sum of odd‑position parts)
 *   E = m – O      (sum of even‑position parts)
 *   a = ⌈k/2⌉
 *   b = ⌊k/2⌋
 *   f = binomial(O‑1, a‑1) * binomial(E‑1, b‑1)   (with binomial(n,k)=0 for k>n)
 *
 *  The result is 0 if parity of i and m differ or if the constraints
 *  O < a or E < b are violated.
 */

#include <stdint.h>
#include <math.h>

#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n  < (r+1)(r+2)/2 */
    idx_t r = (idx_t)((sqrt(1.0 + 8.0 * (double)n) - 1.0) * 0.5);
    /* Adjust r to guarantee the inequality (safe for all n) */
    for (;;) {
        unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
        unsigned __int128 next = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (tri > n) r--;
        else if (next <= n) r++;
        else break;
    }
    /* column index inside the row */
    idx_t start = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t col    = n - start;                 /* 0 <= col <= r */
    if (r == 0) return 1;                     /* a(0) = 1 */

    /* Parameters for the composition count */
    idx_t m   = r + 1;        /* m = row index + 1 */
    idx_t k   = m - col;      /* length of composition */
    /* alternating sum i */
    int64_t i = (int64_t)(2 * (int64_t)col - ((int64_t)r - 1)); /* may be negative */

    /* Parity check */
    if (((m & 1UL) != (int64_t)(i & 1))) return 0;

    /* Compute O and E */
    int64_t sum = i + (int64_t)m;
    /* sum is always even by parity test */
    int64_t O = sum / 2;
    int64_t E = (int64_t)m - O;
    if (O < 0 || E < 0) return 0;

    /* ceil(k/2) and floor(k/2) as signed ints */
    int64_t a_val = (int64_t)(k + 1) / 2;  /* ceil */
    int64_t b_val = (int64_t)(k) / 2;      /* floor */

    if (O < a_val || E < b_val) return 0;  /* not enough sum to fill parts */

    /* Binomial computations (non‑negative arguments guaranteed by checks) */
    seq_t part1 = binomial((idx_t)(O - 1), (idx_t)(a_val - 1));
    seq_t part2;
    if (b_val == 0)
        part2 = 1;
    else
        part2 = binomial((idx_t)(E - 1), (idx_t)(b_val - 1));

    return part1 * part2;
}
