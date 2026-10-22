/*
 * A342237  Table read by upward antidiagonals: T(n,k) is the number of
 *          strings of length k over an n-letter alphabet that begin with a
 *          palindrome of two or more letters; n, k >= 1.
 * https://oeis.org/A342237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by iterating the table T(n,k) defined by T(n,1)
 *            = 0 T(n,2k) = n*T(n,2k-1) + n^k - T(n,k) T(n,2k+1) = n*T(n,2k) +
 *            n^(k+1) - T(n,k+1) and reading the entries by upward
 *            antidiagonals. No simpler closed form is known
 * Range:     u8 n <= 32, u16 n <= 72, u32 n <= 165, u64 n <= 425,
 *            u128 n <= 1160, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, but not used */
#include <stdint.h>   /* for uint64_t if needed */
#include <stdlib.h>   /* for malloc / free */

/* Helper for binary exponentiation of seq_t.
   Works for primitive unsigned types and for arbitrary‑precision types
   that support operator*, +, - and assignment. */
static seq_t power_u(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t pow    = base;
    while (exp) {
        if (exp & 1)
            result *= pow;
        exp >>= 1;
        if (exp)
            pow *= pow;
    }
    return result;
}

/* Sequence: a(n) for OEIS A342237 */
seq_t a(idx_t n) {
    if (n == 0)               /* offset starts at 1; below offset is unspecified */
        return 0;

    /* Find the diagonal s (sum n+k) that contains the n-th entry.
       The triangular numbers count how many entries precede diagonal s. */
    idx_t d = 2;                       /* start with diagonal 2 (n=k=1) */
    while ( ((unsigned __int128)(d-1) * d) / 2 < n )
        ++d;                           /* smallest d with C(d-1) >= n */

    /* Triangular number of previous diagonals */
    unsigned __int128 prev_tri = ((unsigned __int128)(d-2) * (d-1)) / 2;
    idx_t offset = (idx_t)(n - prev_tri);          /* 1 .. d-1   */
    idx_t row    = d - offset;                     /* alphabet size */
    idx_t col    = offset;                         /* string length */

    if (col <= 1)                                 /* T(n,1) = 0 by definition */
        return 0;

    /* Maximum possible col for n up to about 10 000 is < 150.
       Allocate a fixed buffer on the stack to avoid dynamic allocation
       of objects that may be arbitrary‑precision classes. */
    enum { MAX_COL = 200 };
    seq_t T[MAX_COL];
    T[1] = 0;                                     /* base case */

    /* Build T[n][k] sequentially up to the required column. */
    for (idx_t k = 2; k <= col; ++k) {
        if ((k & 1) == 0) {                      /* even k = 2*half */
            idx_t half = k / 2;
            seq_t pow_half = power_u(row, half);
            T[k] = row * T[k-1] + pow_half - T[half];
        } else {                                 /* odd k = 2*half+1 */
            idx_t half = (k - 1) / 2;
            seq_t pow_halfp1 = power_u(row, half + 1);
            T[k] = row * T[k-1] + pow_halfp1 - T[half + 1];
        }
    }
    return T[col];
}
