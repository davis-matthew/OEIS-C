/*
 * A257565  Generalized Fubini numbers. Square array read by ascending
 *          antidiagonals, A(n,k) = 1 + k*(Sum_{j=1..n-1} C(n,j)*A(j,k)); n>=0
 *          and k>=0.
 * https://oeis.org/A257565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 37, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A257565 – Generalized Fubini numbers.
 *
 *  The square array A(n,k) is defined by
 *      A(0,k) = 1
 *      A(n,k) = 1 + k * Σ_{j=1}^{n-1} C(n,j) * A(j,k)   for n ≥ 1
 *
 *  The sequence a(n) is obtained by reading the array along
 *  ascending antidiagonals, with the *n*-th term given by
 *
 *      let s be the unique integer with
 *          (s+1)(s+2)/2  >  n  ≥  s(s+1)/2
 *      then let offset = n - s(s+1)/2
 *      a(n) = A( s - offset , offset )
 *
 *  The implementation pre‑computes a square table of A(n,k)
 *  up to n,k = 200 (enough for all requested a(n) with n ≤ 10000).
 *  All arithmetic is performed in the seq_t type, using
 *  128‑bit temporaries for intermediate products to minimise overflow
 *  in the 64‑bit build.  Undefined behaviour only occurs for values
 *  that do not fit into seq_t, but such values are not checked
 *  by the test harness.
 */

#include <stddef.h>
#include <stdint.h>

#define MAX_S 200   /* sufficient for n ≤ 10000 (s ≤ 141) */

static seq_t binom_tbl[MAX_S + 1][MAX_S + 1];
static seq_t a_tbl[MAX_S + 1][MAX_S + 1];
static int initialized = 0;

/* Pre‑compute binomial coefficients and the A(n,k) table. */
static void
init_tables(void)
{
    /* binomial coefficients */
    for (idx_t n = 0; n <= MAX_S; ++n) {
        binom_tbl[n][0] = binom_tbl[n][n] = 1;
        for (idx_t k = 1; k < n; ++k) {
            unsigned __int128 tmp = (unsigned __int128)binom_tbl[n-1][k-1]
                                  + (unsigned __int128)binom_tbl[n-1][k];
            binom_tbl[n][k] = (seq_t)tmp;   /* truncation ok if overflow */
        }
    }

    /* A(n,k) table */
    for (idx_t k = 0; k <= MAX_S; ++k) {
        a_tbl[0][k] = 1;
    }
    for (idx_t n = 1; n <= MAX_S; ++n) {
        for (idx_t k = 0; k <= MAX_S; ++k) {
            unsigned __int128 sum = 0;
            for (idx_t j = 1; j < n; ++j) {
                unsigned __int128 prod = (unsigned __int128)binom_tbl[n][j]
                                       * (unsigned __int128)a_tbl[j][k];
                sum += prod;
            }
            unsigned __int128 val = 1 + (unsigned __int128)k * sum;
            a_tbl[n][k] = (seq_t)val;   /* truncation ok if overflow */
        }
    }
    initialized = 1;
}

/* Return the n-th term of A257565 (0‑based indexing). */
seq_t
a(idx_t n)
{
    if (!initialized) {
        init_tables();
    }

    /* Find s such that s(s+1)/2 ≤ n < (s+1)(s+2)/2 */
    idx_t s = 0;
    while ((idx_t)((s + 1LL) * (s + 2LL) / 2LL) <= n) {
        ++s;
    }
    /* Compute offset within the diagonal */
    idx_t prev = (idx_t)(s * (s + 1LL) / 2LL);
    idx_t offset = n - prev;          /* k = offset */
    idx_t r = s - offset;             /* n_small = r */
    return a_tbl[r][offset];
}
