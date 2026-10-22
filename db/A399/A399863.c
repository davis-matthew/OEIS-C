/*
 * A399863  Triangle read by rows: T(n,k) = binomial(n+1,2) -
 *          binomial(n-k+1,2) + binomial(n-k,2) - [n-k+1 >
 *          k]*binomial(n-2*k+1,2), k = 1..n, with Iverson brackets.
 * https://oeis.org/A399863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where T(r,k) = binomial(r+1,2) - binomial(r-k+1,2)
 *            + binomial(r-k,2) - [r-k+1 > k] * binomial(r-2*k+1,2) (1‑based
 *            row r, column k)
 * Range:     u8 n <= 262, u16 n <= 9642, u32 n <= 9642, u64 n <= 9642,
 *            u128 n <= 9642, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* binomial(m,2) for m < 2 returns 0 */
static inline seq_t C2(idx_t m)
{
    if (m < 2) return 0;
    return ((seq_t)m * (m - 1)) / 2;
}

/* a(n): triangle read by rows beginning with a(1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – not used */

    /* find row r such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while (( (__int128)r * (r + 1) ) / 2 < n) r++;

    /* compute column k (1‑based) */
    idx_t prefix = ( (__int128)(r - 1) * r ) / 2;   /* entries before row r */
    idx_t k = n - prefix;

    seq_t t1 = C2(r + 1);          /* binomial(r+1,2) */
    seq_t t2 = C2(r - k + 1);      /* binomial(r-k+1,2) */
    seq_t t3 = C2(r - k);          /* binomial(r-k,2) */

    seq_t value = t1 - t2 + t3;    /* accumulating intermediate result */

    /* subtract last term only if condition holds */
    if ((r - k + 1) > k) {
        seq_t t4 = C2(r - 2 * k + 1);  /* binomial(r-2k+1,2) */
        value -= t4;
    }

    return value;
}
