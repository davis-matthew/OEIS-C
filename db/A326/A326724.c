/*
 * A326724  Triangle with Euler (secant) numbers, read by rows, T(n, k) for 0
 *          <= k <= n.
 * https://oeis.org/A326724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the flat index of the triangle of Euler (secant) numbers
 *            defined by T(n,k) = E_{2n} * binomial(n,k) * (-1)^k (0 <= k <=
 *            n), where E_{2n} is the Euler secant number. a(n) returns the
 *            n-th element of the triangle read row by row
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 29, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

#define MAX_E 2000          /* enough for n up to ≈10⁴ */
static seq_t E[MAX_E];     /* Euler secant numbers E_{2m} */
static idx_t e_len = 0;    /* number of computed entries */

/* Compute Euler secant numbers up to index 'need' (i.e., E_{2*need}) */
static void ensure_e_size(idx_t need)
{
    if (need < e_len)
        return;
    if (need >= MAX_E) {
        need = MAX_E - 1;   /* keep within bounds */
    }
    if (e_len == 0) {
        E[0] = (seq_t)1;
        e_len = 1;
    }
    for (idx_t m = e_len; m <= need; ++m) {
        seq_t sum = (seq_t)0;
        idx_t two_m = m << 1;
        for (idx_t k = 0; k < m; ++k) {
            seq_t term = binomial(two_m, k << 1) * E[k];
            sum += term;
        }
        E[m] = -sum;          /* recurrence: E_{2m} = -∑ binom(2m,2k)E_{2k} */
    }
    e_len = need + 1;
}

/* Convert flat index n into row r and column c (0‑based) */
static void find_row_col(idx_t n, idx_t *r, idx_t *c)
{
    idx_t row = 0;
    /* find largest row such that row*(row+1)/2 <= n */
    while (((row + 1) * (row + 2)) / 2 <= n)
        ++row;
    *r = row;
    *c = n - row * (row + 1) / 2;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    idx_t r, c;
    find_row_col(n, &r, &c);
    ensure_e_size(r);

    seq_t coeff = binomial(r, c);
    seq_t sign  = ((c & 1) != 0) ? (seq_t)-1 : (seq_t)1;
    return sign * coeff * E[r];
}
