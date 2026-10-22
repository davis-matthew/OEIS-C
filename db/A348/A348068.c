/*
 * A348068  Coefficient of x^5 in expansion of n!* Sum_{k=0..n} binomial(x,k).
 * https://oeis.org/A348068
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = \sum_{k=0}^{n} \binom{n}{k} \, Stirling1(k,5) \, (n-k)!
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = n * a(n-1) + Stirling1(n,5)   (for n >= 5) */

#include <stdlib.h>

static seq_t *A = NULL;           /* cached sequence values */
static size_t A_cap = 0;          /* allocated capacity for A */
static idx_t  computed = 4;       /* highest index computed so far (a(4)=0) */
static seq_t  current_a = 0;      /* a(computed) */
static seq_t  s[6] = {0};         /* current Stirling numbers s(n,k), k=0..5 */

/* Update Stirling numbers from n-1 to n.
   After the call s[k] holds Stirling1(n,k) for k=0..5. */
static void step_stirling(idx_t n)
{
    /* update in reverse order to avoid using a temporary array */
    for (int k = 5; k >= 1; --k)
        s[k] = s[k-1] - (seq_t)(n-1) * s[k];
    s[0] = 0;
}

/* Ensure that A has room for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) > A_cap) {
        size_t new_cap = A_cap ? A_cap * 2 : 16;
        while ((size_t)(n + 1) > new_cap) new_cap *= 2;
        seq_t *newA = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!newA) abort();          /* out of memory */
        /* zero the newly allocated part */
        for (size_t i = A_cap; i < new_cap; ++i) newA[i] = 0;
        A = newA;
        A_cap = new_cap;
    }
}

/* Lazy initialization of static state. */
static void init_once(void)
{
    if (!A) {
        A_cap = 16;
        A = (seq_t *)calloc(A_cap, sizeof(seq_t));
        if (!A) abort();            /* out of memory */

        /* start with s(0,0)=1, others 0 */
        for (int k = 0; k < 6; ++k) s[k] = 0;
        s[0] = 1;

        current_a = 0;              /* a(4) = 0 */
        computed = 4;
        /* compute Stirling numbers up to n = 4 */
        for (idx_t i = 1; i <= 4; ++i)
            step_stirling(i);
    }
}

seq_t a(idx_t n)
{
    if (n < 5) return 0;          /* values below offset are unspecified */

    init_once();

    if (n > computed) {
        ensure_capacity(n);
        for (idx_t i = computed + 1; i <= n; ++i) {
            step_stirling(i);                     /* compute s(i,5) */
            current_a = (seq_t)i * current_a + s[5]; /* recurrence */
            A[i] = current_a;                     /* cache */
        }
        computed = n;
    }
    return A[n];
}
