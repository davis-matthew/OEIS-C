/*
 * A335748  T(n,k) = (-1)^n*(binomial(2*k,k)/(k+1))*Sum_{j=0..n}
 *          (-1)^j*binomial(k,j)*j^n. Triangle read by rows, T(n, k) for n >=
 *          0 and 0 <= k <= n.
 * https://oeis.org/A335748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A335748 sequence implementation
 *
 * Triangle T(n,k) defined by
 *   T(n,k) = (-1)^n * Catalan(k) * Σ_{j=0..n} (-1)^j * binomial(k,j) * j^n,
 * read by rows: a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
 *
 * The values satisfy the recurrence
 *   T(n,0) = 0 for n>0,
 *   T(n,k) = -k*T(n-1,k) + ( (2k)(2k-1)/(k+1) ) * T(n-1,k-1)   (1)
 * for 1 ≤ k ≤ n.  This recurrence is used for fast table construction.
 *
 * For n up to ~10 000 the maximal row index needed is 141, so a statically
 * sized table of 200×200 entries suffices for all builds.
 */

#include <stdbool.h>

/* Maximal row needed for n ≤ 10 000 (141) – 200 gives a safety margin. */
#define MAX_R 200

/* Static table and initialisation flag. */
static bool _init_done = false;
static seq_t _T[MAX_R + 1][MAX_R + 1];

/* Initialise the triangle once. */
static void _init_triangle(void)
{
    if (_init_done)
        return;
    _init_done = true;

    _T[0][0] = (seq_t)1;          /* T(0,0) = 1 */

    for (idx_t n = 1; n <= MAX_R; ++n) {
        _T[n][0] = (seq_t)0;      /* T(n,0) = 0 for n>0 */
        for (idx_t k = 1; k <= n; ++k) {
            /* term1 = -k * T(n-1,k) */
            seq_t term1 = - (seq_t)k * _T[n - 1][k];

            /* term2 = ((2k)*(2k-1))/(k+1) * T(n-1,k-1) */
            seq_t term2 = _T[n - 1][k - 1];
            term2 *= (seq_t)((idx_t)2 * k);           /* multiply by 2k */
            term2 *= (seq_t)((idx_t)2 * k - 1);       /* multiply by (2k-1) */
            term2 /= (seq_t)(k + 1);                 /* exact division */

            _T[n][k] = term1 + term2;
        }
    }
}

/* Return the nth term of sequence A335748. */
seq_t a(idx_t n)
{
    _init_triangle();

    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((idx_t)(r + 1)) * ((idx_t)(r + 2)) / 2 <= n)
        ++r;

    idx_t k = n - r * (r + 1) / 2;   /* column index in row r */
    return _T[r][k];
}
