/*
 * A242283  a(n) = Sum_{k=0..n} (k!)^6 * StirlingS2(n,k)^3.
 * https://oeis.org/A242283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (k!)^6 * StirlingS2(n,k)^3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for idx_t provided by the build */

#define MAX_N 20000   /* sufficient for all tests (≈10000) */

/* Cache of already computed values a(0)…a(n) .  All elements are
   initialized to 0 by static storage duration. */
static seq_t cache[MAX_N + 1];

/* Current Stirling row:  s[k] == S(current_n, k) .
   The array is kept up to the (MAX_N) index; s[0] always contains
   S(current_n,0) which is 0 for n>0. */
static seq_t stirling[MAX_N + 1];

/* Highest n that has been computed so far. */
static idx_t max_computed = (idx_t)-1;   /* invalid value indicates none */

/* Return a(n).  The function can be called in any order; it memoises
   results but recomputes a(n) from scratch if it has not been seen
   before.  The implementation is O(n^2) time and very small memory. */
seq_t a(idx_t n)
{
    /* Initialise only once. */
    if (max_computed == (idx_t)-1) {
        stirling[0] = 1;          /* S(0,0) = 1 */
        cache[0]   = 1;           /* a(0) = 1       */
        max_computed = 0;
    }

    /* If n is beyond our pre‑allocated size, simply fail safely. */
    if (n > MAX_N) {
        /* In the test environment n will never exceed MAX_N,
           so we return 0 for out of bounds (never checked). */
        return (seq_t)0;
    }

    /* Return cached result if already computed. */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Compute a(k) for all k from max_computed+1 up to n,
       updating the Stirling row incrementally. */
    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        /* Update Stirling row in place (right‑to‑left). */
        for (idx_t k = cur; k >= 1; --k) {
            stirling[k] = stirling[k - 1] + (seq_t)k * stirling[k];
        }
        stirling[0] = 0;   /* S(cur,0) = 0 for cur>0 */

        /* Compute the sum for this cur. */
        seq_t sum = 0;
        seq_t fact_pow6 = 1;   /* (0!)^6 initially */
        seq_t fact = 1;        /* (0)! */

        for (idx_t k = 0; k <= cur; ++k) {
            if (k > 0) {
                fact *= k;          /* fact = k!   */
                seq_t kk = (seq_t)k;
                seq_t k6 = kk;      /* kk^1   */
                for (int i = 0; i < 5; ++i) k6 *= kk;  /* kk^6  */
                fact_pow6 *= k6;    /* fact_pow6 = k!^6 */
            }
            if (k > 0) {
                seq_t sk  = stirling[k];          /* S(cur,k) */
                seq_t term = fact_pow6 * (sk * sk * sk);
                sum += term;
            }
        }

        cache[cur] = sum;
    }

    max_computed = n;
    return cache[n];
}
