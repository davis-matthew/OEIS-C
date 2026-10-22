/*
 * A317839  Möbius transform of A002487, Stern's Diatomic sequence.
 * https://oeis.org/A317839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} μ(d) · A002487(n/d) (Möbius transform)
 * Range:     i8 n <= 1450, i16 n <= 9939, i32 n <= 9939, i64 n <= 9939,
 *            i128 n <= 9939, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mobius.h"   /* provides seq_t mobius(idx_t m) */

/* ---------- Stern's Diatomic sequence (A002487) ----------------- */
static seq_t *stern_vals = NULL;   /* memoized values of A002487 */
static bool  *stern_set  = NULL;   /* flags for initialized entries */
static size_t stern_cap  = 0;       /* current capacity of the arrays */

/* Ensure the memoization tables can store an entry with index ≤ k. */
static void ensure_stern_capacity(idx_t k)
{
    if (k < stern_cap) return;

    size_t new_cap = (size_t)(k + 1);
    stern_vals = (seq_t *)realloc(stern_vals, new_cap * sizeof(seq_t));
    stern_set  = (bool *)realloc(stern_set,  new_cap * sizeof(bool));

    for (size_t i = stern_cap; i < new_cap; ++i) stern_set[i] = false;
    stern_cap = new_cap;
}

/* Recursive, memoized computation of Stern's sequence. */
static seq_t stern_value(idx_t k)
{
    if (k < stern_cap && stern_set[k]) return stern_vals[k];

    if (k == 0) {
        ensure_stern_capacity(1);
        stern_vals[0] = 0;
        stern_set[0] = true;
        return 0;
    }
    if (k == 1) {
        ensure_stern_capacity(2);
        stern_vals[1] = 1;
        stern_set[1] = true;
        return 1;
    }
    if ((k & 1ULL) == 0) {                 /* even */
        idx_t m = k >> 1;
        seq_t val = stern_value(m);
        ensure_stern_capacity(k + 1);
        stern_vals[k] = val;
        stern_set[k]  = true;
        return val;
    } else {                               /* odd */
        idx_t m = k >> 1;
        seq_t val = stern_value(m) + stern_value(m + 1);
        ensure_stern_capacity(k + 1);
        stern_vals[k] = val;
        stern_set[k]  = true;
        return val;
    }
}

/* --------------------------------------------------------------- */
/* Main sequence function: Möbius transform of Stern's sequence.   */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(1) must be non‑zero; the following logic automatically handles it. */
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {   /* iterate over divisors d of n */
        if (n % d == 0) {
            seq_t mu = mobius(d);
            if (mu) {
                seq_t term = stern_value(n / d);
                sum += mu * term;
            }
            idx_t e = n / d;                /* paired divisor e = n/d */
            if (e != d) {
                mu = mobius(e);
                if (mu) {
                    seq_t term = stern_value(n / e);
                    sum += mu * term;
                }
            }
        }
    }
    return sum;
}
