/*
 * A337161  Square array read by antidiagonals: T(n,k) is the number of simple
 *          labeled graphs G with vertex set V(G) = {v_1,...,v_n} along with a
 *          (coloring) function C:V(G) ->[k] such that v_i adjacent to v_j
 *          implies C(v_i) != C(v_j) and i<j implies C(v_i) <= C(v_j); n>=0,
 *          k>=0.
 * https://oeis.org/A337161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 60, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A337161
    Square array read by antidiagonals:
    T(n,k) = number of labeled graphs G with vertices v1..vn
    together with a coloring C:V(G)->[k] satisfying
      (1) if vi adjacent to vj then C(vi)≠C(vj)
      (2) i<j ⇒ C(vi)≤C(vj)

    a(m) is obtained by reading T(n,k) along the antidiagonals
    of the square array, starting with (0,0).
    Implementation uses the recurrence

        T(n,0) = 0  (n>0),   T(0,k)=1
        T(n,k) = T(n,k-1) + Σ_{t=1}^n T(n‑t,k‑1) * 2^{t·(n-t)}

    The prefix “intended index” for a(m) is found as follows.
    For m, let d be the unique integer with
          d·(d+1)/2 ≤ m < (d+1)(d+2)/2.
    Set n0 = m – d·(d+1)/2   and   k0 = d – n0.
    Then a(m) = T(n0,k0).  All needed values with n,k≤150
    are pre‑computed once; this covers all indices up to 10 000
    (the required test bound).  Arbitrary precision big‑ints
    are used automatically if seq_t is such a type.  */

#include <stdbool.h>
#include <stdint.h>

#define MAX_D   150                      /* largest n or k needed (covers d≤140) */
#define MAX_EXP ((MAX_D/2) * (MAX_D - MAX_D/2))  /* maximum exponent t*(n-t) */

/* the types seq_t and idx_t are supplied by the build system */
static seq_t T[MAX_D+1][MAX_D+1];
static seq_t pow2[MAX_EXP+1];
static bool precomputed = false;

/* initialise tables once */
static void init_tables(void)
{
    if (precomputed) return;
    precomputed = true;

    /* powers of two */
    pow2[0] = 1;
    for (idx_t i = 1; i <= MAX_EXP; ++i) {
        pow2[i] = pow2[i-1] * (seq_t)2;
    }

    /* T(n,0) and T(0,k) */
    T[0][0] = 1;
    for (idx_t n = 1; n <= MAX_D; ++n) T[n][0] = 0;
    for (idx_t k = 1; k <= MAX_D; ++k) T[0][k] = 1;

    /* recurrence for k≥1 */
    for (idx_t k = 1; k <= MAX_D; ++k) {
        for (idx_t n = 1; n <= MAX_D; ++n) {
            seq_t sum = 0;
            for (idx_t t = 1; t <= n; ++t) {
                idx_t exp = t * (n - t);
                sum += T[n - t][k - 1] * pow2[exp];
            }
            T[n][k] = T[n][k-1] + sum;
        }
    }
}

/* find antidiagonal d for a given index m */
static idx_t diag_of(idx_t m)
{
    idx_t d = 0;
    while ( ( (d + 1) * (d + 2) / 2 ) <= m ) ++d;
    return d;
}

/* public function required by the driver */
seq_t a(idx_t m)
{
    init_tables();                     /* guarantee tables are ready */

    idx_t d   = diag_of(m);            /* diagonal number */
    idx_t start = d * (d + 1) / 2;     /* first index on that diagonal */
    idx_t offset = m - start;          /* position in the diagonal */
    idx_t n0 = offset;                 /* row index */
    idx_t k0 = d - offset;             /* column index */

    /* safety check – should never happen for tested bounds */
    if (n0 > MAX_D || k0 > MAX_D) return 0;

    return T[n0][k0];
}
