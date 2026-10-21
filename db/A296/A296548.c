/*
 * A296548  Triangle read by rows: T(n,k) is the number of diagonalizable n X
 *          n matrices over GF(2) that have rank k, n >= 0, 0 <= k <= n.
 * https://oeis.org/A296548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 39, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A296548 – number of diagonalizable n×n matrices over GF(2)
 *  with rank k (flattened triangle).  The value a(n) is read from the
 *  triangle  T(r,k) = A022166(r,k) * 2^(k(r-k))  where r and k are
 *  obtained from the index n as described in the OEIS entry.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

/* ---------- helper: Gaussian binomial coefficient for q = 2 ---------- */
static seq_t gaussian_binom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;

    /* reduce k to the smaller side of the symmetry */
    if (k > n - k)
        k = n - k;

    seq_t res = 1;
    /* product over i = 1 .. k of (2^(n+1-i)-1)/(2^i-1) */
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = big_pow((seq_t)2, (seq_t)(n + 1 - i)) - 1;
        seq_t den = big_pow((seq_t)2, (seq_t)i) - 1;
        res = res * num / den;           /* exact division at each step */
    }
    return res;
}

/* ---------- helper: find the row r for index n -------------------- */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    while (((__int128)high * (high + 1)) / 2 <= n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;   /* upper mid */
        if (((__int128)mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ------------------------------ main function ------------------------------ */
seq_t a(idx_t n)
{
    /* Locate the triangle cell (r,k) corresponding to the flat index n */
    idx_t r = find_row(n);
    idx_t before = ((__int128)r * (r + 1)) / 2;   /* terms before row r   */
    idx_t k = n - before;                         /* column in row r      */

    /* Handle the boundary cases first */
    if (k == 0 || k == r)
        return 1;

    /* Compute T(r,k) = GaussianBinomial(r,k) * 2^(k*(r-k)) */
    seq_t g = gaussian_binom(r, k);
    idx_t expo = k * (r - k);
    seq_t p2 = big_pow((seq_t)2, (seq_t)expo);
    return g * p2;
}
