/*
 * A382995  Square array A(n,k), n >= 1, k >= 1, read by antidiagonals
 *          downwards, where A(n,k) = Sum_{d|n} phi(n/d) * (-k)^(d-1).
 * https://oeis.org/A382995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the nth element of the antidiagonal reading of the
 *            square array A(n,k) defined by A(n,k) = ∑_{d|n} φ(n/d) *
 *            (-k)^(d-1) where φ is Euler's totient function
 * Range:     i8 n <= 32, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code computes the coordinates (row,col) = (n,k) corresponding to the
   position in the antidiagonal ordering and evaluates the divisor sum. */

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Helper: Euler's totient function for an integer up to 64 bits.     */
/* ------------------------------------------------------------------ */
static inline seq_t phi(idx_t x)
{
    seq_t result = (seq_t)x;
    idx_t m = x;
    for (idx_t p = 2; (uint64_t)p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            seq_t p_s = (seq_t)p;
            result = result / p_s * (p_s - 1);
        }
    }
    if (m > 1) {
        seq_t m_s = (seq_t)m;
        result = result / m_s * (m_s - 1);
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Integer exponentiation for seq_t base with an idx_t exponent.      */
/* Uses binary exponentiation.                                         */
/* ------------------------------------------------------------------ */
static inline seq_t pow_seq_int(seq_t base, idx_t exp)
{
    seq_t res = 1;
    seq_t cur = base;
    while (exp > 0) {
        if (exp & 1) res *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Find the diagonal number d (starting at 2) that contains position pos. */
/* Minimal d such that d*(d-1)/2 >= pos.                               */
/* ------------------------------------------------------------------ */
static inline idx_t find_diagonal(idx_t pos)
{
    idx_t lo = 1, hi = pos + 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 t = (unsigned __int128)(mid - 1) * mid / 2;
        if (t >= pos)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;        /* d */
}

/* ------------------------------------------------------------------ */
/* Compute A(n,k) using the divisor-sum definition.                    */
/* ------------------------------------------------------------------ */
static inline seq_t compute_A(idx_t n, idx_t k)
{
    seq_t sum = 0;
    seq_t base = -(seq_t)k;
    for (idx_t d = 1; (unsigned __int128)d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t term = phi(n / d) * pow_seq_int(base, d - 1);
            sum += term;
            idx_t other = n / d;
            if (other != d) {
                term = phi(n / other) * pow_seq_int(base, other - 1);
                sum += term;
            }
        }
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/* Main function: returns the nth term of sequence A382995.             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not defined, but guard */
    idx_t d = find_diagonal(n);                           /* diagonal */
    unsigned __int128 triPrev = (unsigned __int128)(d - 2) * (d - 1) / 2;
    idx_t offset = n - (idx_t)triPrev - 1;                /* position in diagonal */
    idx_t row = offset + 1;                               /* n coordinate */
    idx_t col = d - row;                                  /* k coordinate */
    return compute_A(row, col);
}
