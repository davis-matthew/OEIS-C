/*
 * A397349  L.g.f. Sum_{n>=1} a(n)*x^n/n = log(1+x + Sum_{n>=2} 3*n/(3*n^2 -
 *          1) * a(n)*x^n ).
 * https://oeis.org/A397349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A397349
 * Formula: a(1)=1;
 * For n≥2: a(n) = (3*n^2-1) *
 *            ( a(n-1) + Σ_{m=2}^{n-1} (3*m/(3*m^2-1)) * a(m)*a(n-m) )
 * (All divisions are exact by construction.)
 */

#include <stdlib.h>

static seq_t *A = NULL;   /* a[1..]  (1‑based: A[1] = a(1)) */
static seq_t *F = NULL;   /* F[m] = (3*m) * (a(m)/(3*m^2-1))   for m≥2   */
static idx_t  computed = 0;  /* largest n for which A[n] is computed */
static size_t cap = 0;        /* allocated capacity (≥ computed+1) */

static void ensure_capacity(size_t need)
{
    if (cap > need) return;
    size_t new_cap = cap ? cap << 1 : 1;
    while (new_cap <= need) new_cap <<= 1;

    if (A == NULL) {
        A = (seq_t *)malloc(new_cap * sizeof(seq_t));
        F = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        F = (seq_t *)realloc(F, new_cap * sizeof(seq_t));
    }
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, guard against underflow */

    ensure_capacity((size_t)n + 1);

    if (computed == 0) {
        A[1] = (seq_t)1;    /* a(1) = 1   */
        computed = 1;
    }

    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t sum = A[k - 1];   /* a(k-1) */

        for (idx_t m = 2; m < k; ++m) {
            sum += F[m] * A[k - m];
        }

        seq_t k_seq = (seq_t)k;
        seq_t denom = (seq_t)3 * k_seq * k_seq - (seq_t)1;
        A[k] = denom * sum;

        /* compute F[k] for future use: F[k] = (3*k) * (a(k)/denom) */
        F[k] = (seq_t)3 * k_seq * (A[k] / denom);
    }

    computed = n;
    return A[n];
}
