/*
 * A365623  T(n,k) is the number of parking functions of length n with cars
 *          parking at most k spots away from their preferred spot; square
 *          array T(n,k), n>=0, k>=0, read by downward antidiagonals.
 * https://oeis.org/A365623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = 1 if n = 0 T(n,k) = Σ_{i=0}^{n-1} [ C(n-1,i) *
 *            min(i+1,k+1) * T(i,k) * T(n-1-i,k) ] for n > 0 a(n) is the
 *            flattened sequence obtained by reading the square array T(n,k)
 *            along downward antidiagonals: a(0)=T(0,0), a(1)=T(0,1),
 *            a(2)=T(1,0), a(3)=T(0,2), …
 * Range:     u8 n <= 25, u16 n <= 51, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The build supplies the integer types seq_t and idx_t.
    Sequence values are returned of type seq_t.            */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/*  One‑dimensional storage for the triangular table T[n][k]   */
static seq_t *T = NULL;          /* pointer to current table          */
static idx_t  maxM = 0;          /* largest antidiagonal that was
                                   computed and stored (m)           */

/* ------------------------------------------------------------------ */
/* Helper: allocate (m+1)×(m+1) table, compute all T[n][k] for
   0 ≤ n,k ≤ m.                                                    */
static void compute_table_up_to(idx_t m)
{
    /*  Base case: m = 0  ------------------------------------------- */
    if (m == 0) {
        if (T == NULL) {                    /* first time we need it  */
            T = (seq_t*)malloc(sizeof(seq_t));
            if (!T) return;                 /* allocation failed      */
            T[0] = (seq_t)1;                /* T(0,0) = 1             */
        }
        maxM = 0;
        return;
    }

    /*  No need to recompute if already computed up to m          */
    if (m <= maxM) return;

    /*  Allocate new larger table ------------------------------------ */
    if (T != NULL) free(T);                  /* free old, if any       */
    size_t size = (size_t)(m + 1) * (m + 1);
    T = (seq_t*)malloc(size * sizeof(seq_t));
    if (!T) return;                           /* allocation failed      */

    /*  Macro to index into the 1‑D array                         */
    #define T_at(nn,kk) ( T[(size_t)(nn) * (m + 1) + (kk)] )

    /*  Initialise base row T(0,k)=1  -------------------------------- */
    for (idx_t kk = 0; kk <= m; ++kk) T_at(0,kk) = (seq_t)1;

    /*  Populate the rest of the table  ---------------------------- */
    for (idx_t kk = 0; kk <= m; ++kk) {
        for (idx_t nn = 1; nn <= m; ++nn) {
            seq_t sum = (seq_t)0;
            for (idx_t ii = 0; ii <= nn - 1; ++ii) {
                seq_t bin  = binomial(nn - 1, ii);   /* C(nn-1,ii)   */
                seq_t minv = (ii + 1 < kk + 1) ?
                               (seq_t)(ii + 1) : (seq_t)(kk + 1);
                seq_t prod = bin * minv;
                prod      = prod * T_at(ii   , kk);
                prod      = prod * T_at(nn-1-ii, kk);
                sum       = sum + prod;
            }
            T_at(nn, kk) = sum;
        }
    }

    maxM = m;                                   /* remember size      */
    #undef T_at
}

/* ------------------------------------------------------------------ */
/* Convert a flattened index n to the corresponding antidiagonal
   coordinates (row n, column k).  The index is counted starting at
   zero: a(0)=T(0,0), a(1)=T(0,1), a(2)=T(1,0), …              */
static void index_to_pair(idx_t idx, idx_t *row, idx_t *col)
{
    /*  Find largest m with m(m+1)/2 ≤ idx  -------------------------- */
    idx_t m = 0;
    while ((m * (m + 1)) / 2 <= idx) ++m;
    --m;

    /*  Position on that antidiagonal  ------------------------------ */
    idx_t offset = idx - (m * (m + 1)) / 2;
    *row = offset;
    *col = m - offset;
}

/* ------------------------------------------------------------------ */
/* Public function required by the problem statement.  Computes the
   n-th term of the flattened array a(n).  Results are memoised so
   that subsequent calls with smaller and larger indices are fast.   */
seq_t a(idx_t n)
{
    /*  Determine the antidiagonal needed for this index  ----------- */
    idx_t m = 0;
    while ((m * (m + 1)) / 2 <= n) ++m;
    --m;                       /* largest n or k that may occur */

    /*  Ensure the table has been computed up to this size.         */
    compute_table_up_to(m);

    /*  Convert the flat index to (row, col) on antidiagonal m.     */
    idx_t r, c;
    index_to_pair(n, &r, &c);

    /*  Fetch the value from the stored table.                       */
    return T[(size_t)r * (m + 1) + c];
}
