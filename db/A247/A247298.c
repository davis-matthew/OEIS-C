/*
 * A247298  Number of weighted lattice paths B(n) having no uudd strings.
 * https://oeis.org/A247298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; * For n ≥ 1: * a(n) = a(n‑1) + a(n‑2) – a(n‑6) + c(n)
 *            * (with a(k)=0 for k<0) * c(n) = ∑_{i=0}^{n‑3} a(i)⋅a(n‑3‑i)
 *            (c(n)=0 for n<3) * This recurrence follows from the functional
 *            equation * G = 1 + zG + z²G + z³G(G – z³)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* seq_t and idx_t are provided by the build system. */

static seq_t *A = NULL;         /* a[0..] */
static seq_t *C = NULL;         /* convolution: C[k] = ∑_{i=0}^{k} a[i]⋅a[k‑i] */
static size_t cache_size = 0;   /* allocated capacity */
static size_t computed = 0;     /* number of elements computed (i.e. we have A[0..computed‑1]) */

/* enlarge the cache so that it can hold indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache_size > (size_t)n) return;

    size_t new_size = cache_size ? cache_size : 1;
    while (new_size <= (size_t)n) new_size <<= 1;

    seq_t *tmpA = (seq_t *)realloc(A, new_size * sizeof(seq_t));
    seq_t *tmpC = (seq_t *)realloc(C, new_size * sizeof(seq_t));
    if (!tmpA || !tmpC) return;                /* allocation failed – leave unchanged */

    if (cache_size < new_size) {
        /* initialise the new slots to zero */
        for (size_t i = cache_size; i < new_size; ++i) {
            tmpA[i] = 0;
            tmpC[i] = 0;
        }
    }

    A = tmpA;
    C = tmpC;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* make sure the arrays are large enough */
    ensure_cache(n);
    if (cache_size == 0) return (seq_t)0; /* unlikely, but guard against failure */

    /* initialise the base of the sequence if not done yet */
    if (computed == 0) {
        A[0] = (seq_t)1;
        C[0] = A[0] * A[0];                 /* C[0] = a0*a0 */
        computed = 1;
    }

    /* compute a[computed] … a[n] */
    for (idx_t m = computed; m <= n; ++m) {
        seq_t term1 = (m >= 1) ? A[m-1] : 0;          /* a(m‑1)   */
        seq_t term2 = (m >= 2) ? A[m-2] : 0;          /* a(m‑2)   */
        seq_t term3 = (m >= 6) ? A[m-6] : 0;          /* a(m‑6)   */
        seq_t cterm = (m >= 3) ? C[m-3] : 0;          /* c(m)     */

        seq_t val = term1 + term2 - term3 + cterm;
        A[m] = val;

        /* compute the convolution entry C[m] = ∑ a[i]*a[m‑i]   (i=0..m) */
        seq_t sum = 0;
        for (idx_t i = 0; i <= m; ++i) {
            sum += A[i] * A[m-i];
        }
        C[m] = sum;
    }

    computed = n + 1;   /* we have now computed up to index n inclusive */
    return A[n];
}
