/*
 * A286310  G.f.: 1 + Sum_{n>=1} a(n)*x^n/(1 - x^n) = 1/(1 - x/(1 - 2*x/(1 -
 *          3*x/(1 - 4*x/(1 - ...))))).
 * https://oeis.org/A286310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sum_{d|n} a(d) = (2n-1)!! (double factorial of odd numbers).
 *            Compute a(n) by recurrence: a(1)=1, and for n>1, a(n) = (2n-1)!!
 *            - sum_{d|n, d<n} a(d)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function below assumes seq_t and idx_t are predefined types by the build system.  */
#include <stdlib.h>

static seq_t *S_cache = NULL;   /* (2n-1)!!  */
static seq_t *A_cache = NULL;   /* a(n)      */
static size_t cache_cap = 0;    /* current capacity */

/*  Make sure all arrays are allocated up to at least n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;          /* 0..n indices */
    if (need <= cache_cap)
        return;

    /* Expand both arrays */
    seq_t *newS = (seq_t *)realloc(S_cache, need * sizeof(seq_t));
    seq_t *newA = (seq_t *)realloc(A_cache, need * sizeof(seq_t));
    if (!newS || !newA) { /* allocation failure – abort to avoid undefined behaviour */
        abort();
    }
    S_cache = newS;
    A_cache = newA;

    /* Zero‑initialize new elements (allocation by malloc/realloc does not do this) */
    for (size_t i = cache_cap; i < need; ++i) {
        S_cache[i] = 0;
        A_cache[i] = 0;
    }

    /* Set base values for index 0 (unused) */
    if (cache_cap == 0) {
        S_cache[0] = 1;   /* by convention */
        A_cache[0] = 0;
    }

    /* Compute (2n-1)!! for new indices and a(n) by the recurrence */
    for (size_t i = cache_cap; i < need; ++i) {
        if (i == 0) {
            S_cache[0] = 1;
            A_cache[0] = 0;
            continue;
        }

        /* S[i] = S[i-1] * (2*i - 1) */
        seq_t factor = (seq_t)(2 * i - 1);
        S_cache[i] = S_cache[i - 1] * factor;

        /* Sum over proper divisors */
        seq_t sum = 0;
        for (size_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                if (d < i)          sum += A_cache[d];
                size_t d2 = i / d;
                if (d2 != d && d2 < i)  sum += A_cache[d2];
            }
        }

        /* a[i] = S[i] - sum of a(d) for d|i, d<i */
        A_cache[i] = S_cache[i] - sum;
    }

    cache_cap = need;
}

/*  Public function required by the test harness.  */
seq_t a(idx_t n)
{
    if (n == 0)   /* Undefined for 0; return 0 to avoid UB. */
        return 0;
    ensure_cache(n);
    return A_cache[(size_t)n];
}
