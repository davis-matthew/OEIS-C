/*
 * A391251  Numbers k such that the square of Stern polynomial B(k,x) is also
 *          a Stern polynomial.
 * https://oeis.org/A391251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 101, u64 n <= 101, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A391251: Numbers k such that B(k,x)^2 is a Stern polynomial.
   The sequence is defined by testing the equality
     B(k^2,x) == B(k,x)^2
   for the Stern polynomials B(k,x).  The function below evaluates
   B(k,x) by an iterative algorithm that runs in O(log k) time. */

#include <stdint.h>
#include <stdbool.h>

/* Locate the most significant set bit of a 64‑bit number (k>0). */
static unsigned msb_pos(idx_t n)
{
    return 63 - __builtin_clzll(n);
}

/* Evaluate the Stern polynomial B(n,x) for a given integer x.
   It returns the value as a seq_t.  Complexity: O(log n). */
static seq_t eval_B(idx_t n, seq_t x)
{
    if (n == 0)                /* B(0)=0 */
        return 0;

    seq_t P = 1;               /* B(1)=1 at start */
    seq_t Q = x;               /* B(2)=x */

    unsigned m = msb_pos(n);   /* floor(log2 n) */
    for (int pos = (int)m - 1; pos >= 0; --pos) {
        bool bit = (n >> pos) & 1ULL;
        seq_t Pold = P;
        seq_t Qold = Q;

        if (!bit) {            /* even step: n = 2 * n' */
            P = Pold * x;
            Q = Pold + Qold;
        } else {               /* odd step: n = 2 * n' + 1 */
            P = Pold + Qold;
            Q = Qold * x;
        }
    }
    return P;                  /* B(n,x) */
}

/* Test if a given k satisfies B(k^2,x) == B(k,x)^2 for all required x. */
static bool ok_for_k(idx_t k)
{
    unsigned m   = msb_pos(k);          /* floor(log2 k) */
    unsigned deg = 2 * m;              /* degree bound for B(k^2,x) */
    unsigned __int128 k2_128 = (unsigned __int128)k * k;
    idx_t k2 = (idx_t)k2_128;          /* k^2 fits in 64 bits for our use */

    for (unsigned x = 0; x <= deg; ++x) {
        seq_t v1 = eval_B(k,  (seq_t)x);
        seq_t v2 = eval_B(k2, (seq_t)x);
        if (v1 * v1 != v2)
            return false;
    }
    return true;
}

/* Main sequence function: return the n-th term (1‑based). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        if (ok_for_k(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
