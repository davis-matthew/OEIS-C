/*
 * A309537  Total number of Fibonacci parts in all compositions of n.
 * https://oeis.org/A309537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n-1} a(k) + Σ_{f Fibonacci, 1≤f≤n} C(n-f) where
 *            C(m) = 1 for m = 0, otherwise C(m) = 2^{m-1}
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *A      = NULL;     /* a(n) values          */
static seq_t *comp   = NULL;     /* C(n)  = compositions of n */
static idx_t  computed = 0;      /* highest n computed   */
static size_t cap      = 0;      /* allocated array size */
static seq_t  curr_sum = 0;      /* Σ_{k=0}^{computed} a(k) */

/* Fibonacci numbers (distinct) */
static idx_t *fibs      = NULL;   /* array of Fibonacci numbers */
static size_t fib_cap   = 0;      /* allocated capacity of fibs */
static size_t fib_count = 0;      /* number of Fibonacci numbers stored */

/* Ensure capacity of A and comp arrays for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= cap) return;
    size_t newcap = (size_t)(n + 1) * 2;
    if (newcap < 2) newcap = 2;
    seq_t *newA  = (seq_t*)realloc(A, newcap * sizeof(seq_t));
    seq_t *newC  = (seq_t*)realloc(comp, newcap * sizeof(seq_t));
    if (!newA || !newC) exit(EXIT_FAILURE);
    A      = newA;
    comp   = newC;
    cap    = newcap;
}

/* Ensure capacity of the Fibonacci array. */
static void ensure_fib_capacity(size_t new_cap)
{
    if (new_cap <= fib_cap) return;
    idx_t *newf = (idx_t*)malloc(new_cap * sizeof(idx_t));
    if (!newf) exit(EXIT_FAILURE);
    for (size_t i = 0; i < fib_count; ++i) newf[i] = fibs[i];
    free(fibs);
    fibs = newf;
    fib_cap = new_cap;
}

/* Grow the Fibonacci list until it contains all Fibonacci numbers ≤ limit. */
static void extend_fibs_to(idx_t limit)
{
    if (fibs == NULL) {
        fib_cap = 16;
        fibs = (idx_t*)malloc(fib_cap * sizeof(idx_t));
        if (!fibs) exit(EXIT_FAILURE);
        fibs[0] = 1;
        fibs[1] = 2;
        fib_count = 2;
    }
    while (fibs[fib_count-1] <= limit) {
        idx_t next = fibs[fib_count-1] + fibs[fib_count-2];
        if (next > limit) break;
        if (fib_count == fib_cap) ensure_fib_capacity(fib_cap * 2);
        fibs[fib_count++] = next;
    }
}

/* Compute all values up to n inclusive. */
static void compute_upto(idx_t n)
{
    if (n <= computed) return;
    ensure_capacity(n);
    extend_fibs_to(n);

    for (idx_t i = computed + 1; i <= n; ++i) {
        /* C(i) */
        if (i == 0) {
            comp[0] = (seq_t)1;
        } else if (i == 1) {
            comp[1] = comp[0];                    /* C(1) = 1 */
        } else {
            comp[i] = comp[i-1] + comp[i-1];       /* 2 * C(i-1) */
        }

        /* Σ_{f Fibonacci ≤ i} C(i-f) */
        seq_t sumFib = 0;
        for (size_t j = 0; j < fib_count && fibs[j] <= i; ++j) {
            sumFib = sumFib + comp[i - fibs[j]];
        }

        /* a(i) = Σ_{k=0}^{i-1} a(k) + sumFib */
        seq_t val = curr_sum + sumFib;
        A[i] = val;
        curr_sum = curr_sum + val;
    }
    computed = n;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (A == NULL) {
        /* Initial allocation for indices 0 through 1 */
        size_t init_cap = 8;
        A     = (seq_t*)malloc(init_cap * sizeof(seq_t));
        comp  = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!A || !comp) exit(EXIT_FAILURE);
        cap = init_cap;

        /* base values */
        A[0]    = (seq_t)0;
        comp[0] = (seq_t)1;
        curr_sum = (seq_t)0;
        computed = 0;
    }

    compute_upto(n);
    return A[n];
}
