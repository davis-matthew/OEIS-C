/*
 * A274273  Number of noncomposite areas of a Venn diagram for n multisets.
 * https://oeis.org/A274273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274273: Number of noncomposite areas of a Venn diagram
   for n multisets.  Implements the closed form
   a(n) = 1 - 2*F(n) + F(n+1)   (for n ≥ 1),
   where F(k) is the ordered Bell number (A000670) with recurrence
   F(0)=1 and
        F(m) = Σ_{k=0}^{m-1} binom(m, k) * F(k).
   The function is memoised so that calls may arrive in any order.  */

#include <stdlib.h>
#include <stddef.h>

/* ---------- local cache of ordered Bell numbers (F) ---------- */
static idx_t  F_len   = 0;      /* number of stored values   */
static size_t F_cap   = 0;      /* capacity of the array    */
static seq_t *F_vals  = NULL;   /* array of F[0 .. F_len-1] */

/* ensure the cache has at least new_cap elements */
static void ensure_F_capacity(size_t new_cap)
{
    if (new_cap <= F_cap) return;
    F_cap = new_cap;
    F_vals = (seq_t *)realloc(F_vals, F_cap * sizeof(seq_t));
    if (!F_vals) abort();
}

/* compute all F values up to target (inclusive) */
static void compute_F_up_to(idx_t target)
{
    /* already have it up to target ? */
    if (target < F_len) return;

    /* start with F[0] = 1 if not yet initialised */
    if (F_len == 0) {
        ensure_F_capacity(1);
        F_vals[0] = (seq_t)1;
        F_len = 1;
    }

    /* compute subsequent values sequentially */
    for (idx_t m = F_len; m <= target; ++m) {
        seq_t sum = (seq_t)0;
        seq_t C   = (seq_t)1;       /* C(m,0) = 1 */
        for (idx_t k = 0; k < m; ++k) {
            sum += C * F_vals[k];
            /* update C to C(m, k+1) = C * (m - k) / (k + 1) */
            if (k + 1 < m) {
                C = (C * (seq_t)(m - k)) / (seq_t)(k + 1);
            }
        }
        ensure_F_capacity(F_len + 1);
        F_vals[F_len++] = sum;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* need F[n] and F[n+1] */
    compute_F_up_to(n + 1);

    /* a(n) = 1 - 2*F(n) + F(n+1) */
    seq_t val = ((seq_t)1 + F_vals[n + 1]) - ((seq_t)2 * F_vals[n]);
    return val;
}
