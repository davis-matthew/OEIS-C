/*
 * A362825  Number of ordered triples of involutions on [n] that pairwise
 *          commute.
 * https://oeis.org/A362825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k∈{1,2,4,8}} C(k) * binom(n-1,k-1) * a(n-k) where the
 *            constant C(k) are C(1) = 1 C(2) = 7 C(4) = 42 C(8) = 5040 (These
 *            are the values of f_k in the EGF expansion exp(x + 7*x^2/2 +
 *            7*x^4/4 + x^8/8).)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Memoised results for a(0)..a(computed) */
static seq_t *A    = NULL;
static size_t cap = 0;            /* allocated size of A                */
static idx_t computed = 0;       /* highest index already computed     */

static const seq_t C1 = (seq_t)1;
static const seq_t C2 = (seq_t)7;
static const seq_t C4 = (seq_t)42;
static const seq_t C8 = (seq_t)5040;

/* Ensure A has at least 'needed' slots. */
static void ensure_capacity(size_t needed)
{
    if (cap >= needed)
        return;
    size_t new_cap = (cap == 0 ? 8 : cap);
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!tmp) abort();                         /* allocation failure */
    A = tmp;
    cap = new_cap;
}

/* Return binom(n,r) for r ≤ 7 (works for all n ≤ 10000). */
static seq_t small_binom(idx_t n, idx_t r)
{
    if (r > n) return 0;
    if (r > n - r) r = n - r;                  /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= r; ++i) {
        result *= (seq_t)(n - r + i);
        result /= (seq_t)i;                    /* division is exact */
    }
    return result;
}

/* Compute a(0)..a(n) if not already done. */
static void compute_up_to(idx_t n)
{
    if (!A) {
        /* Ensure we have room for at least n+1 elements */
        ensure_capacity((size_t)n + 1 > 8 ? (size_t)n + 1 : 8);
        A[0] = (seq_t)1;
        computed = 0;
    } else if ((size_t)n + 1 > cap) {
        ensure_capacity((size_t)n + 1);
    }

    if (n <= computed)
        return;

    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t sum = 0;

        /* k = 1 */
        sum += A[i - 1];

        /* k = 2 */
        if (i >= 2) {
            seq_t bin = small_binom(i - 1, 1);   /* binom(i-1,1) = i-1 */
            sum += bin * C2 * A[i - 2];
        }

        /* k = 4 */
        if (i >= 4) {
            seq_t bin = small_binom(i - 1, 3);   /* binom(i-1,3) */
            sum += bin * C4 * A[i - 4];
        }

        /* k = 8 */
        if (i >= 8) {
            seq_t bin = small_binom(i - 1, 7);   /* binom(i-1,7) */
            sum += bin * C8 * A[i - 8];
        }

        A[i] = sum;
    }
    computed = n;
}

/* Public API */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return A[n];
}
