/*
 * A286933  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of continued fraction 1/(1 - k*x/(1 -
 *          k*x^2/(1 - k*x^3/(1 - k*x^4/(1 - k*x^5/(1 - ...)))))).
 * https://oeis.org/A286933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A286933:  a(n) is obtained by reading the square array A(n,k)
    (n>=0,k>=0) [indexed by antidiagonals] where column k
    is the expansion of the continued fraction

          1/(1 - k*x/(1 - k*x^2/(1 - k*x^3/(1 - k*x^4/(1 - …))))

    The value a(n) is the coefficient of x^{row} in column {column},
    where (row,column) is obtained from n as follows:
      n  =  row + column  +  row*column/2   (reading by antidiagonals).
    This implementation evaluates the coefficient by
    the rational representation

          N(x)/D(x)
    with

          N(x) = Σ_{j≥0} (-k)^j * x^{j(j+1)}  / Π_{i=1}^{j} (1-x^i)
          D(x) = Σ_{j≥0} (-k)^j * x^{j^2}      / Π_{i=1}^{j} (1-x^i).

    For a given n we first find its row and column coordinates,
    then compute the required coefficient using integer arithmetic
    (signed __int128 for intermediate values) and cache the result
    for all pairs (row,column) with 0 ≤ row, column ≤ 200.
    The algorithm runs in O(row * sqrt(row)) which is well below the
    2‑second limit for n up to about 10000.
*/

#include <stdint.h>
#include <stdbool.h>

/* cache limits: for n up to about 10 000 row and column never exceed 200 */
#define MAXI 200
#define MAXK 200

static seq_t  cache  [MAXI+1][MAXK+1];
static bool   filled [MAXI+1][MAXK+1];

/* ------------------------------------------------------------------ */
/*  coefficient of x^{row} in column 'k'  (0≤row≤200, 0≤k≤200)  */
static seq_t coeff_row_k(idx_t row, idx_t k)
{
    /* fast result for column 0 */
    if (k == 0) {
        return (row == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* return cached value if available */
    if (filled[row][k]) return cache[row][k];

    const idx_t Nmax = row;                 /* only terms up to row are needed */

    /* temporary arrays – maximum size 201 for all cases          */
    __int128 D[201] = {0};
    __int128 N[201] = {0};
    __int128 Q[201] = {0};
    __int128 A[201] = {0};

    /* iterate over j for which j(j+1) ≤ Nmax or j^2 ≤ Nmax   */
    __int128 pow = 1;                       /* (-k)^j  */
    for (idx_t j = 0; ; ++j)
    {
        idx_t shiftD = j*j;
        idx_t shiftN = j*(j+1);

        if (shiftD > Nmax && shiftN > Nmax && j > 0) break;

        /* compute Q = 1 / Π_{i=1}^{j} (1-x^i)  = partitions with parts ≤ j */
        if (j == 0) {
            Q[0] = 1;
            for (idx_t i = 1; i <= Nmax; ++i) Q[i] = 0;
        } else {
            for (idx_t i = 0; i <= Nmax; ++i) Q[i] = 0;
            Q[0] = 1;
            for (idx_t part = 1; part <= j; ++part) {
                for (idx_t n = part; n <= Nmax; ++n) {
                    Q[n] += Q[n - part];
                }
            }
        }

        /* add contributions to D and N */
        if (shiftD <= Nmax) {
            for (idx_t n = shiftD; n <= Nmax; ++n) {
                D[n] += pow * Q[n - shiftD];
            }
        }
        if (shiftN <= Nmax) {
            for (idx_t n = shiftN; n <= Nmax; ++n) {
                N[n] += pow * Q[n - shiftN];
            }
        }

        /* update pow = -pow * k  for next j */
        pow = -pow * (__int128)k;
    }

    /* series division: N = D * A  →  solve for A  */
    A[0] = N[0];   /* N[0] == 1 */
    for (idx_t i = 1; i <= Nmax; ++i) {
        __int128 temp = N[i];
        for (idx_t s = 1; s <= i; ++s) {
            temp -= D[s] * A[i - s];
        }
        A[i] = temp;
    }

    seq_t result = (seq_t)A[Nmax];
    cache[row][k] = result;
    filled[row][k] = true;
    return result;
}

/* ------------------------------------------------------------------ */
/*  public sequence generator                                         */
seq_t a(idx_t n)
{
    /* find (row,column) from n (antidiagonal ordering) */
    idx_t t = 0;
    while ( ((t + 1) * (t + 2)) / 2 <= n ) ++t;        /* minimal t with (t+1)(t+2)/2 > n */

    idx_t prev = t * (t + 1) / 2;         /* N(t-1) */
    idx_t pos  = n - prev;                /* position within diagonal t */
    idx_t row   = pos;                    /* row index  */
    idx_t col   = t - pos;                /* column index */

    return coeff_row_k(row, col);
}
