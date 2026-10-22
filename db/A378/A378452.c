/*
 * A378452  Dirichlet inverse of A007875, where A007875(n) = phi(2^omega(n)).
 * https://oeis.org/A378452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{d|n, d<n} A007875(n/d) * a(d), where A007875(n) =
 *            φ(2^ω(n)). a(1) = 1
 * Range:     i8 n <= 8658, i16 n <= 8658, i32 n <= 8658, i64 n <= 8658,
 *            i128 n <= 8658, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

static seq_t *A = NULL;           /* cached a(n) values (1‑indexed)   */
static bool   *computed = NULL;   /* true if A[i] already computed   */
static idx_t   cap = 0;           /* capacity of A/computed arrays   */
static idx_t   max_computed = 0;  /* highest n that has been computed */

/* compute A007875(n) = φ(2^ω(n)) */
static inline seq_t g_val(idx_t n)
{
    if (n <= 1u) return (seq_t)1;
    int r = count_distinct_primes((uint64_t)n);
    if (r <= 0) return (seq_t)1;          /* n==1, theoretically not reached */
    return ((seq_t)1 << (r - 1));          /* φ(2^r) = 2^{r-1} for r>=1      */
}

/* ensure arrays allocated up to n (1‑based) */
static void ensure_capacity(idx_t n)
{
    if (cap < n + 1) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *newA = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
        bool  *newC = (bool*)realloc(computed, new_cap * sizeof(bool));
        if (!newA || !newC) {
            /* allocation failure – terminate */
            exit(1);
        }
        A = newA;
        computed = newC;
        for (idx_t i = cap; i < new_cap; ++i) computed[i] = false;
        cap = new_cap;
    }
}

/* compute a(n) using the recurrence, extending the cache as needed */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, guard against */
    ensure_capacity(n);
    if (n == 1) {                  /* base case */
        if (!computed[1]) {
            A[1] = (seq_t)1;
            computed[1] = true;
            max_computed = 1;
        }
        return A[1];
    }
    if (computed[n]) return A[n];   /* already computed */

    /* compute values from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d != 0) continue;
            idx_t d2 = i / d;           /* the paired divisor */
            /* divisor d < i, always true for i>1 */
            seq_t g = g_val(d2);
            sum += g * A[d];
            if (d2 != d && d2 < i) {    /* also include the complementary divisor */
                g = g_val(d);
                sum += g * A[d2];
            }
        }
        A[i] = -sum;
        computed[i] = true;
    }
    max_computed = n;
    return A[n];
}
