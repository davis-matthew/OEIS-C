/*
 * A303675  Triangle read by rows: coefficients in the sum of odd powers as
 *          expressed by Faulhaber's theorem, T(n, k) for n >= 1, 1 <= k <= n.
 * https://oeis.org/A303675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (1/m) * Σ_{j=0..m} (-1)^j * binomial(2*m, j) *
 *            (m-j)^(2*n) where m = n-k+1 The sequence a(n) is the triangle
 *            read by rows: a(n) = T(r,k) where r = smallest integer with
 *            r(r+1)/2 ≥ n, and k = n - r(r-1)/2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 21, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper: integer square‑root of an unsigned 64‑bit integer.          */
/* Returns floor( sqrt(n) ).                                         */
/* ------------------------------------------------------------------ */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            res = mid;
            low = mid + 1;
        }
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Helper: multiplicative binomial coefficient C(n,k).                */
/* Uses the identity C(n,k)=Π_{i=1..k} (n-k+i)/i.                     */
/* ------------------------------------------------------------------ */
static seq_t binom_u64(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Helper: exponentiation by squaring: compute base^exp.              */
/* Both base and exp are seq_t; multiplication/truncating division    */
/* are valid on seq_t.                                                */
/* ------------------------------------------------------------------ */
static seq_t pow_seq(seq_t base, seq_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if ((exp & 1) != 0) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Main sequence function as required.                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* --- find row r and position k in that row ------------------- */
    idx_t r = (floor_sqrt_u64(8*n - 7) + 1) / 2;
    idx_t prev = r * (r - 1) / 2;         /* terms before row r   */
    idx_t k = n - prev;                   /* 1‑based column index */

    /* --- compute T(r,k) ------------------------------------------- */
    idx_t m = r - k + 1;                  /* m = r‑k+1            */
    seq_t exp = (seq_t)(2 * r);           /* exponent 2*r         */

    seq_t pos_sum = 0;                    /* sum of even j terms   */
    seq_t neg_sum = 0;                    /* sum of odd  j terms   */

    idx_t two_m = 2 * m;

    for (idx_t j = 0; j <= m; ++j) {
        seq_t bin = binom_u64(two_m, j);          /* C(2m, j)          */
        seq_t base = (seq_t)(m - j);              /* m-j               */
        seq_t powv = pow_seq(base, exp);          /* (m-j)^(2r)        */
        seq_t term = bin * powv;                  /* binomial * power  */

        if ((j & 1) == 0)
            pos_sum += term;
        else
            neg_sum += term;
    }

    seq_t sum = pos_sum - neg_sum;               /* overall alternating sum */
    seq_t result = sum / (seq_t)m;                /* divide by m            */
    return result;
}
