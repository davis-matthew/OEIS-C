/*
 * A344479  Square array T(n,k), n >= 1, k >= 1, read by antidiagonals, where
 *          T(n,k) = Sum_{1 <= x_1, x_2, ..., x_k <= n} gcd(x_1, x_2, ...,
 *          x_k).
 * https://oeis.org/A344479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = Sum_{j=1..row} φ(j) * ⌊row/j⌋^col, where (row,col) are
 *            obtained from n by reading the infinite square array
 *            T(n,k)=∑_{j=1..n} φ(j)⋅⌊n/j⌋^k by antidiagonals (OEIS A344479)
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 157, u64 n <= 412,
 *            u128 n <= 1092, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"   /* seq_t pow_seq(seq_t a, int k); */

static idx_t  max_phi   = 0;        /* highest n for which tables exist     */
static uint64_t *phi_tbl = NULL;   /* φ[i] for 0..max_phi                  */
static seq_t   *pref_tbl = NULL;   /* prefix sums of φ in seq_t             */

/* Ensure φ table and its prefix sums are valid up to n */
static void
ensure_phi(idx_t n)
{
    if (n <= max_phi) return;

    /* Free old tables */
    free(phi_tbl);
    free(pref_tbl);

    /* Allocate new tables */
    phi_tbl = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    pref_tbl = (seq_t *)malloc((n + 1) * sizeof(seq_t));

    /* Initialise φ[i] = i */
    for (idx_t i = 0; i <= n; ++i) phi_tbl[i] = i;

    /* Sieve to compute φ */
    for (idx_t i = 2; i <= n; ++i) {
        if (phi_tbl[i] == i) {            /* i is prime */
            for (idx_t j = i; j <= n; j += i) {
                phi_tbl[j] -= phi_tbl[j] / i;
            }
        }
    }

    /* Build prefix sums, storing as seq_t */
    pref_tbl[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        pref_tbl[i] = pref_tbl[i - 1] + (seq_t)phi_tbl[i];
    }

    max_phi = n;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    /* Find anti‑diagonal index c such that (c-1)*c/2 >= n */
    idx_t c = 2;
    while ((c - 1) * c / 2 < n) ++c;

    /* Numbers preceding this diagonal */
    idx_t prev = (c - 2) * (c - 1) / 2;
    idx_t offset = n - prev - 1;          /* 0‑based offset on the c‑th diagonal */

    /* Map to (row,col) = (n,k) in the square array */
    idx_t row = offset + 1;
    idx_t col = (c - 1) - offset;

    /* Prepare φ values up to 'row' */
    ensure_phi(row);

    seq_t result = 0;

    /* Sum over j = 1..row using summation by ranges of equal floor(row/j) */
    idx_t l = 1;
    while (l <= row) {
        idx_t d   = row / l;                 /* common value of ⌊row/j⌋ for j in [l,r]  */
        idx_t r   = row / d;                 /* last j with this value                  */
        seq_t sum_phi = pref_tbl[r] - pref_tbl[l - 1];
        seq_t pow_d   = pow_seq((seq_t)d, (int)col);
        result += sum_phi * pow_d;
        l = r + 1;
    }

    return result;
}
