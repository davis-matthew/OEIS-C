/*
 * A366117  Expansion of (1/x) * Series_Reversion( x*(1+x+x^3)/(1+x)^3 ).
 * https://oeis.org/A366117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/3)} (-1)^k *
 *            binomial(n+k,k) * binomial(2*n-k+2,n-3*k)
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 14, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* For bool, not strictly needed but allowed */

/* helper to compute binomial coefficient C(a,b) for 0 <= b <= a */
static seq_t _binom(idx_t a, idx_t b)
{
    if (b > a) return 0;
    /* use symmetry to reduce the number of iterations */
    if (b > a - b) b = a - b;
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        /* Multiply first to keep the division exact */
        res = res * (seq_t)(a - b + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)(-1)) return 0;  /* defensive for unsigned wrap‑around where n-1 would cause UB; not expected to be called */
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = _binom(n + k, k);
        seq_t t2 = _binom((2 * n - k + 2), (n - 3 * k));
        seq_t term = t1 * t2;
        if (k & 1)
            sum -= term;
        else
            sum += term;
    }
    return sum / (seq_t)(n + 1);
}
