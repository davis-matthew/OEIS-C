/*
 * A275198  Triangle, read by rows, formed by reading Pascal's triangle
 *          (A007318) mod 14.
 * https://oeis.org/A275198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r, k) mod 14, where r = row index containing n
 *            and k = column index within that row (0‑based). The row/column
 *            indices are determined from the triangular numbers:
 *            triangular(r) = r*(r+1)/2
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stddef.h>

static inline idx_t triangular(idx_t r)
{
    return (idx_t)((unsigned __int128)r * (r + 1) / 2);
}

/* Precomputed binomial coefficients modulo 7 for 0 ≤ n,k ≤ 6 */
static const unsigned char C7[7][7] = {
    {1,0,0,0,0,0,0},
    {1,1,0,0,0,0,0},
    {1,2,1,0,0,0,0},
    {1,3,3,1,0,0,0},
    {1,4,6,4,1,0,0},
    {1,5,3,3,5,1,0},
    {1,6,1,6,1,6,1}
};

static inline int binom_mod7(idx_t n, idx_t k)
{
    int res = 1;
    while (n > 0 || k > 0) {
        int ni = n % 7;
        int ki = k % 7;
        if (ki > ni) return 0;
        res = (res * C7[ni][ki]) % 7;
        n /= 7;
        k /= 7;
    }
    return res;
}

static inline int binom_mod2(idx_t n, idx_t k)
{
    return ((k & ~n) == 0) ? 1 : 0;
}

static inline int binom_mod14(idx_t n, idx_t k)
{
    int r2 = binom_mod2(n, k);
    int r7 = binom_mod7(n, k);
    int res = r7;
    if ((res & 1) != r2) res += 7;   /* adjust parity to satisfy the mod‑2 residue */
    return res % 14;
}

/* Locate the row r that contains index n */
static inline idx_t find_row(idx_t n)
{
    /* Initial guess using the inverse triangular function */
    long double x = sqrt((long double)8.0 * (long double)n + 1.0);
    idx_t r = (idx_t)((x - 1.0L) / 2.0L);

    /* Correct possible off‑by‑one errors due to floating‑point rounding */
    while (triangular(r + 1) <= n) ++r;
    while (triangular(r) > n) --r;
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t prev_terms = triangular(r);
    idx_t k = n - prev_terms;   /* column index */
    int mod14 = binom_mod14(r, k);
    return (seq_t)mod14;
}
