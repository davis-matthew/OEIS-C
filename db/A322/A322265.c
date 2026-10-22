/*
 * A322265  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = numerator of Sum_{j=1..n} 1/j^k.
 * https://oeis.org/A322265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 31, u32 n <= 50, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = numerator of the sum Σ_{j=1}^{row(n)} 1/j^{col(n)},
   where (row(n),col(n)) are the coordinates of the n-th cell
   when the 2‑dimensional array A(n,k)=numerator of Σ_{j=1}^n 1/j^k
   is read by antidiagonals (n ≥ 1, k ≥ 0).  */

#include "../utils/seq_gcd.h"
#include "../utils/seq_pow.h"

static seq_t sum_numer(idx_t n, idx_t k)
{
    seq_t num = 0;     /* running sum numerator  (initially 0) */
    seq_t den = 1;     /* running sum denominator (initially 1) */

    for (idx_t j = 1; j <= n; ++j)
    {
        /* denominator of the term 1/j^k */
        seq_t term_den = seq_pow((seq_t)j, k);

        /* add 1/term_den to num/den  */
        seq_t new_num = num * term_den + den;
        seq_t new_den = den * term_den;

        /* reduce by gcd to keep intermediates small */
        seq_t g = seq_gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;

        num = new_num;
        den = new_den;
    }
    return num;   /* numerator of the sum */
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal (sum = s) containing the n-th term.
       For s, there are s terms:
           index 1..s   → (n=1,k=s-1)
           index 2..s   → (n=2,k=s-2)
           ...
           index s      → (n=s,k=0)          */
    idx_t s = 1;
    idx_t tri = 1;                /* tri = s*(s+1)/2  */
    while (tri < n)
    {
        ++s;
        tri += s;                 /* update triangular number */
    }

    idx_t before = (s - 1) * s / 2;      /* indices before this diagonal */
    idx_t pos = n - before - 1;          /* 0‑based position within diagonal */
    idx_t row = pos + 1;                 /* n coordinate in the array */
    idx_t col = s - 1 - pos;             /* k coordinate in the array */

    return sum_numer(row, col);
}
