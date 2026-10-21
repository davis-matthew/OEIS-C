/*
 * A285595  Sum T(n,k) of the k-th entries in all blocks of all set partitions
 *          of [n]; triangle T(n,k), n>=1, 1<=k<=n, read by rows.
 * https://oeis.org/A285595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 91, u64 n <= 276,
 *            u128 n <= 820, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A285595 – Sum of k‑th entries in all blocks of all set partitions of [n].
 *
 *  The sequence a(n) is obtained by flattening the triangle T(n,k) of sums of
 *  the k‑th entries of every block over all set partitions of [n].
 *  Formally, for n≥1 and 1≤k≤n
 *
 *      T(n,k) = k * Σ_{t=0}^{n-k} binom(n+1, k+1+t) * B(n-k-t)
 *
 *  where B(m) is the m‑th Bell number.
 *  Then a(n) is the element T(r,c) where r is the smallest integer with
 *  r(r+1)/2 ≥ n and c = n - r(r-1)/2.
 *
 *  The implementation precomputes Bell numbers and the required rows of the
 *  triangle only up to the maximum r needed.  All computations use only
 *  integer arithmetic on the build‑specific types seq_t and idx_t.
 *
 *  The sequence is defined for n≥1; values for n<1 are undefined.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* ---------- constants ---------- */
#define MAX_R 1000              /* should cover all n used in the tests */

/* ---------- static storage ---------- */
static seq_t T[MAX_R + 1][MAX_R + 1];   /* triangle values, 1-based indexing */
static idx_t T_computed = 0;            /* largest row of T that has been filled */

static seq_t bell[MAX_R + 1];           /* Bell numbers, 0-based indexing */
static idx_t bell_computed = 0;         /* largest Bell number computed so far */

/* ---------- helper: compute Bell numbers up to m ---------- */
static void init_bell_up_to(idx_t m)
{
    /* Guarantee that bell[0] is available. */
    if (bell_computed == 0) {
        bell[0] = (seq_t)1;
    }
    while (bell_computed < m) {
        idx_t n = bell_computed + 1;    /* next Bell number to compute */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= n - 1; ++k) {
            /* B_n = Σ_{k=0}^{n-1} C(n-1, k) * B_k  (for n≥1) */
            sum += binom(n - 1, k) * bell[k];
        }
        bell[n] = sum;
        ++bell_computed;
    }
}

/* ---------- helper: compute triangle T up to row r ---------- */
static void init_T_up_to(idx_t r)
{
    if (T_computed >= r) return;

    /* Bell numbers up to r-1 are needed for the formula.  r≥1 here. */
    init_bell_up_to(r - 1);

    for (idx_t n = T_computed + 1; n <= r; ++n) {
        for (idx_t k = 1; k <= n; ++k) {
            seq_t sum = (seq_t)0;
            idx_t t_max = n - k;
            for (idx_t t = 0; t <= t_max; ++t) {
                sum += binom(n + 1, k + 1 + t) * bell[n - k - t];
            }
            T[n][k] = (seq_t)k * sum;
        }
    }
    T_computed = r;
}

/* ---------- main function required by the build ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1, but return 0 for safety */

    /* Find the row r so that r(r+1)/2 ≥ n */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) ++r;

    idx_t prev = r * (r - 1) / 2;   /* number of elements in rows before r */
    idx_t c = n - prev;             /* column index (1‑based) */

    /* Ensure the required data are available. */
    init_T_up_to(r);

    return T[r][c];
}
