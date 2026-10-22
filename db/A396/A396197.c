/*
 * A396197  Triangle read by rows: T(n, k) = (-1)^k*binomial(n, k) * (2*F(n -
 *          k) + F(n - k + 2)) where F(n) are the Fibonacci numbers.
 * https://oeis.org/A396197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = flatten(T) where T(n,k) = (-1)^k * binomial(n,k) * (
 *            2*F(n-k) + F(n-k+2) ), F(0)=0, F(1)=1. (OEIS A396197)
 * Range:     i8 n <= 21, i16 n <= 80, i32 n <= 306, i64 n <= 1143,
 *            i128 n <= 4495, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

/* fast doubling for Fibonacci numbers: 
   computes F(n) and F(n+1) into *fn and *fn1 using seq_t types. */
static void fib_fast(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    fib_fast(n >> 1, fn, fn1);               // fn=F(k), fn1=F(k+1) where k=floor(n/2)
    seq_t a = *fn;                          // F(k)
    seq_t b = *fn1;                         // F(k+1)
    seq_t c = a * ((seq_t)2 * b - a);       // F(2k)
    seq_t d = a * a + b * b;                // F(2k+1)
    if (n & 1) {
        *fn  = d;                            // F(2k+1)
        *fn1 = c + d;                       // F(2k+2)
    } else {
        *fn  = c;                            // F(2k)
        *fn1 = d;                           // F(2k+1)
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* Determine the row r containing index n */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }

    /* offset of first element in that row */
    idx_t offset = (r * (r + 1)) / 2;
    idx_t k = n - offset;        /* column index in row r */
    idx_t m = r - k;             /* n-k in the formula */

    /* Fibonacci numbers F(m) and F(m+2) */
    seq_t fm, fm1;          /* fm = F(m), fm1 = F(m+1) */
    fib_fast(m, &fm, &fm1);
    seq_t fm2;              /* F(m+2) = FM1 + FM */
    fm2 = fm + fm1;

    /* binomial coefficient C(r,k) */
    seq_t comb = binomial(r, k);

    /* Compute the term */
    seq_t term = comb * ((seq_t)2 * fm + fm2);

    /* Apply sign (-1)^k */
    if (k & 1u) {
        term = -term;
    }

    return term;
}
