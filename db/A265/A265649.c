/*
 * A265649  Triangle of coefficients T(n,k) of polynomials p(n,x) =
 *          Sum_{k=0..n} T(n,k)*x^k where T(0,0) = 1, and T(n,k) = 0 for k < 0
 *          or k > n, and T(n,k) = T(n-1,k-1) + (2*n-1+k)*T(n-1,k) for n > 0
 *          and 0 <= k <= n.
 * https://oeis.org/A265649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 27, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A265649 – the “triangle of coefficients” T(n,k)
   where
        T(0,0) = 1 ,
        T(n,k) = T(n-1,k-1) + (2*n-1+k)*T(n-1,k)   for n>0, 0≤k≤n.
   The sequence a(n) is the triangle filled row‑by‑row:
        a(0)=T(0,0),
        a(1)=T(1,0), a(2)=T(1,1),
        a(3)=T(2,0), a(4)=T(2,1), a(5)=T(2,2), …
   For a given index n we locate the row r and column c with
           r(r+1)/2 ≤ n < (r+1)(r+2)/2,
   then c = n – r(r+1)/2.
   The values are computed on demand and cached, so repeated calls are
   fast. */

#include <stdint.h>
#include <stdlib.h>

static seq_t **T_rows = NULL;   /* array of pointers to each row of T */
static idx_t  T_max_row = (idx_t)-1;  /* highest row that has been built */

static void build_next_row(idx_t new_row) {
    /* new_row > 0, previous rows exist up to new_row-1 */
    idx_t i   = new_row;
    size_t len  = (size_t)(i + 1);
    seq_t *prev = T_rows[i-1];
    seq_t *row  = (seq_t*)malloc(len * sizeof(seq_t));
    if (!row) abort();  /* out of memory */

    /* T(i,0) = (2*i-1)*T(i-1,0) */
    row[0] = (2*i - 1) * prev[0];

    /* for 1 ≤ k ≤ i-1 */
    for (idx_t k = 1; k < i; ++k) {
        row[k] = prev[k-1] + (2*i - 1 + k) * prev[k];
    }
    /* T(i,i) = T(i-1,i-1) */
    row[i] = prev[i-1];

    T_rows[i] = row;
}

seq_t a(idx_t n) {
    /* determine the row r that contains the term a(n) */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    /* column index within the row */
    idx_t c = n - r * (r + 1) / 2;

    /* initialise structure if first call */
    if (!T_rows) {
        T_rows = (seq_t**)malloc(sizeof(seq_t*));
        if (!T_rows) abort();
        /* row 0 */
        T_rows[0] = (seq_t*)malloc(sizeof(seq_t));
        if (!T_rows[0]) abort();
        T_rows[0][0] = 1;
        T_max_row = 0;
    }

    /* grow cache up to required row r */
    while (r > T_max_row) {
        /* grow array of pointers */
        size_t new_cap = (size_t)(T_max_row + 2);  /* enough for next row */
        T_rows = (seq_t**)realloc(T_rows, new_cap * sizeof(seq_t*));
        if (!T_rows) abort();
        ++T_max_row;
        build_next_row(T_max_row);
    }

    return T_rows[r][c];
}
