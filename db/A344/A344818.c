/*
 * A344818  a(n) = Sum_{k=1..n} floor(n/k) * (-3)^(k-1).
 * https://oeis.org/A344818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k) * (-3)^(k-1) Equivalent
 *            definition: a(n) = Sum_{k=1..n} Sum_{d | k} (-3)^(d-1)
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 41, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maximum n expected to be called (the problem guarantees n ≤ ~10000). */
#define MAX_CAP 200000

#include <stdint.h>

/* Static tables for memoization.  They are enlarged only once. */
static seq_t   pow_minus3[MAX_CAP + 1];     /* pow_minus3[i] = (-3)^(i-1), 1‑based   */
static seq_t   divisor_sum[MAX_CAP + 1];    /* Sum_{d|k} (-3)^(d-1) for each k      */
static seq_t   a_cache[MAX_CAP + 1];        /* a(k) cached                          */
static idx_t   pow_cap   = 0;               /* highest index of pow_minus3 filled   */
static idx_t   a_cap     = 0;               /* highest index of a_cache computed    */

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure n is within our pre‑allocated bounds. */
    if (n > MAX_CAP) {
        /* For n larger than MAX_CAP the caller will need a different
           implementation.  Here we simply compute the value directly
           using a naive loop (O(n²)).  This covers the rare case when
           a user asks for a very large n. */
        seq_t result = 0;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term = 1;                    /* (-3)^(k-1) */
            for (idx_t i = 1; i < k; ++i)
                term = term * (seq_t)(-3);     /* multiply by -3 each step */
            idx_t q = n / k;
            result += term * (seq_t)q;
        }
        return result;
    }

    /* ----------------------------------------------------------------
       Compute powers of -3 up to n.
       ---------------------------------------------------------------- */
    if (n > pow_cap) {
        for (idx_t i = pow_cap + 1; i <= n; ++i) {
            if (i == 1)
                pow_minus3[i] = (seq_t)1;               /* (-3)^0 = 1 */
            else
                pow_minus3[i] = pow_minus3[i - 1] * (seq_t)(-3);
        }
        pow_cap = n;
    }

    /* ----------------------------------------------------------------
       Compute divisor sums and the sequence a(k) up to n.
       We recompute the divisor sums from scratch each time we extend
       the range; this is O(n log n) and fast enough for n ≤ 200000.
       ---------------------------------------------------------------- */
    if (n > a_cap) {
        /* Compute divisor sums */
        for (idx_t i = 1; i <= n; ++i)
            divisor_sum[i] = (seq_t)0;

        for (idx_t d = 1; d <= n; ++d) {
            seq_t val = pow_minus3[d];
            for (idx_t m = d; m <= n; m += d)
                divisor_sum[m] += val;
        }

        /* Compute a(k) cumulatively */
        a_cache[0] = (seq_t)0;
        for (idx_t k = 1; k <= n; ++k)
            a_cache[k] = a_cache[k - 1] + divisor_sum[k];

        a_cap = n;
    }

    return a_cache[n];
}
