/*
 * A241475  Triangle t(n,r) = s(n,r)*s(n,r+1), where s(n,r) =
 *          lcm(n,n-1,...,n-r+1)/lcm(1,2,...,r-1,r), n >= 1 and 0 <= r < n.
 * https://oeis.org/A241475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = t(k,r) where * k = smallest integer with k*(k+1)/2 >= n
 *            (row number) * r = n - (k-1)*k/2 - 1 (0‑based column) * t(k,r) =
 *            s(k,r) * s(k,r+1) * s(k,r) = lcm(k,k-1,…,k-r+1) / lcm(1,2,…,r) *
 *            s(k,0) is defined to be 1. * * The sequence is the flattened
 *            rows of this triangle
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"  /* seq_t gcd(seq_t a, seq_t b) */

/* Compute lcm(a,b) using gcd; all operations are performed in seq_t. */
static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    return a / gcd(a, b) * b;
}

/* Compute s(k,r) as defined above.  For r = 0 the result is 1. */
static seq_t s_val(idx_t k, idx_t r)
{
    if (r == 0) {
        return (seq_t)1;
    }

    seq_t num = (seq_t)1;
    for (idx_t i = 0; i < r; ++i) {
        num = lcm_seq(num, (seq_t)(k - i));
    }

    seq_t den = (seq_t)1;
    for (idx_t i = 1; i <= r; ++i) {
        den = lcm_seq(den, (seq_t)i);
    }

    return num / den;   /* division is exact by definition */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {          /* undefined for n = 0; return 0 safely */
        return (seq_t)0;
    }

    /* Find the row k such that k*(k+1)/2 >= n (binary search). */
    idx_t low  = 1;
    idx_t high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t k = low;  /* row number (1‑based) */

    /* Compute column index r (0‑based). */
    idx_t prev = (k - 1) * k / 2;        /* terms before this row */
    idx_t r    = n - prev - 1;           /* 0 <= r < k */

    /* Compute t(k,r) = s(k,r) * s(k,r+1). */
    seq_t part1 = s_val(k, r);
    seq_t part2 = s_val(k, r + 1);
    return part1 * part2;
}
