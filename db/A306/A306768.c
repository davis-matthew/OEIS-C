/*
 * A306768  G.f. A(x) satisfies: A(x) = x*exp(-A(-x) + A(-x^2)/2 - A(-x^3)/3 +
 *          A(-x^4)/4 - A(-x^5)/5 + ...).
 * https://oeis.org/A306768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 15, i32 n <= 31, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A306768
 *
 * Recurrence:
 *   a(0) = 0
 *   a(1) = 1
 *   a(n+1) = (1 / n) * Σ_{k=1..n} ( Σ_{d|k} (-1)^{k/d + d} * d * a(d) ) * a(n-k+1)
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array, seq_alloc_new_array */
#include "../utils/seq_free_array.h"   /* seq_free_array */

/* Static storage for the sequence and the auxiliary S(k) values */
static seq_t *a_vals = NULL;          /* a_vals[i] holds a(i)           */
static seq_t *S_vals = NULL;          /* S_vals[i] holds S(i)           */
static size_t cap = 0;                /* current array capacity         */
static idx_t max_computed = 0;        /* largest n for which a_vals[0..n] are defined */

/* Forward declaration of helper */
static seq_t compute_S(idx_t k);

/* Ensure arrays can hold indices up to n (inclusive) */
static void
ensure_cap(idx_t n)
{
    if (cap > (size_t)n)
        return;                       /* already big enough */

    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *new_a = alloc_seq_array(new_cap);
    seq_t *new_S = alloc_seq_array(new_cap);

    /* Copy existing values */
    for (size_t i = 0; i <= (size_t)max_computed; ++i) {
        new_a[i] = a_vals ? a_vals[i] : 0;
        new_S[i] = S_vals ? S_vals[i] : 0;
    }

    /* Free old arrays */
    if (a_vals) seq_free_array(a_vals);
    if (S_vals) seq_free_array(S_vals);

    a_vals = new_a;
    S_vals = new_S;
    cap = new_cap;
}

/* Compute S(k) = Σ_{d|k} (-1)^{k/d + d} * d * a(d) */
static seq_t
compute_S(idx_t k)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= k / d; ++d) {
        if (k % d == 0) {
            idx_t d1 = d;
            idx_t d2 = k / d;
            seq_t sign1 = (((k / d1 + d1) & 1) ? -1 : 1);
            sum += sign1 * (seq_t)d1 * a_vals[d1];
            if (d1 != d2) {
                seq_t sign2 = (((k / d2 + d2) & 1) ? -1 : 1);
                sum += sign2 * (seq_t)d2 * a_vals[d2];
            }
        }
    }
    return sum;
}

/* Public function: compute a(n) */
seq_t
a(idx_t n)
{
    /* Ensure storage can hold up to n */
    ensure_cap(n);

    /* Initialize a(0) and S(0) if not yet done */
    if (max_computed == 0) {
        a_vals[0] = 0;
        S_vals[0] = 0;    /* not used, but defined */
    }

    if (n == 0)
        return a_vals[0];

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            a_vals[1] = 1;
            S_vals[1] = compute_S(1);
        } else {
            seq_t sum = 0;
            for (idx_t k = 1; k < i; ++k) {
                sum += S_vals[k] * a_vals[i - k];
            }
            a_vals[i] = sum / (seq_t)(i - 1);
            S_vals[i] = compute_S(i);
        }
    }

    max_computed = n;
    return a_vals[n];
}
