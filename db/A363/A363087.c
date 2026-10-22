/*
 * A363087  G.f. A(x) satisfies: A(x) = x - x^2 * exp(A(x) - A(x^2)/2 +
 *          A(x^3)/3 - A(x^4)/4 + ...).
 * https://oeis.org/A363087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 38, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A363087
 *  a(1) =  1,  a(2) = -1
 *  For n ≥ 3:
 *     a(n) = (1/(n-2)) * Σ_{k=1}^{n-2} ( Σ_{d|k} (-1)^(k/d+1) · d · a(d) ) · a(n-k)
 */

#include <stdlib.h>
#include <string.h>

/* Memoised storage */
static seq_t *A = NULL;        /* a[1 .. max_n]  */
static seq_t *F = NULL;        /* F[1 .. max_n-2]  */
static idx_t  N_alloc = 0;     /* current allocation size (number of slots) */
static idx_t  computed = 0;    /* largest index for which a has been computed */

/* Expand storage to contain indices up to `up_to` (inclusive). */
static void ensure_capacity(idx_t up_to)
{
    if (up_to + 1 <= N_alloc) return;          /* already large enough */

    idx_t new_len = up_to + 1;                  /* indices 0 .. up_to */

    /* Reallocate A */
    if (A == NULL)
        A = (seq_t*)malloc(new_len * sizeof(seq_t));
    else
        A = (seq_t*)realloc(A, new_len * sizeof(seq_t));
    if (new_len > N_alloc)                     /* zero new slots */
        memset(A + N_alloc, 0, (new_len - N_alloc) * sizeof(seq_t));

    /* Reallocate F */
    if (F == NULL)
        F = (seq_t*)malloc(new_len * sizeof(seq_t));
    else
        F = (seq_t*)realloc(F, new_len * sizeof(seq_t));
    if (new_len > N_alloc)
        memset(F + N_alloc, 0, (new_len - N_alloc) * sizeof(seq_t));

    /* Update F for newly available indices (those larger than
     * (computed == 0 ? 0 : computed-2)).
     */
    idx_t old_max_F = (computed >= 3) ? (computed - 2) : (idx_t)0;
    idx_t new_max_F = (up_to >= 3)      ? (up_to - 2)    : (idx_t)0;

    if (new_max_F > old_max_F) {
        idx_t start_k = old_max_F + 1;               /* first index not yet processed */
        for (idx_t d = 1; d <= computed; ++d) {
            idx_t first = ((start_k + d - 1) / d) * d;    /* first multiple of d ≥ start_k */
            for (idx_t k = first; k <= new_max_F; k += d) {
                idx_t side = k / d;
                seq_t sign = (side % 2 == 0) ? (seq_t)-1 : (seq_t)1;
                F[k] += sign * ((seq_t)d) * A[d];
            }
        }
    }

    /* Compute all a[m] for m = computed+1 .. up_to and update F. */
    for (idx_t m = computed + 1; m <= up_to; ++m) {
        if (m == 1) { A[m] = (seq_t)1; continue; }
        if (m == 2) { A[m] = (seq_t)-1; continue; }

        /* m ≥ 3: evaluate recurrence */
        seq_t sum = 0;
        for (idx_t k = 1; k <= m - 2; ++k)
            sum += F[k] * A[m - k];
        A[m] = sum / (seq_t)(m - 2);

        /* Update F for multiples of m */
        if (up_to >= 3) {
            for (idx_t t = m; t <= up_to - 2; t += m) {
                idx_t side = t / m;
                seq_t sign = (side % 2 == 0) ? (seq_t)-1 : (seq_t)1;
                F[t] += sign * ((seq_t)m) * A[m];
            }
        }
    }

    computed = up_to;
    N_alloc = new_len;
}

/* Public entry point ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n ≤ 0 */
    ensure_capacity(n);
    return A[n];
}
