/*
 * A307059  Expansion of 1/(2 - Product_{k>=1} (1 - x^k)).
 * https://oeis.org/A307059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 45, i16 n <= 98, i32 n <= 186, i64 n <= 371,
 *            i128 n <= 740, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A307059
    Expansion of 1/(2 - Π_{k>=1} (1 - x^k)).
    Recurrence:
        a(0) = 1
        For n ≥ 1:  a(n) = Σ_{k=1..n} q(k) * a(n‑k)
    where q(k) is the coefficient of x^k in Π_{k>=1} (1 - x^k)
      (Euler's pentagonal theorem: q(k) = Σ_{m>0, g(m)=k} (-1)^m,
       with g(m) = m(3m±1)/2).
    This implementation memoises both the q(k) array and the
    a(n) array, using dynamic reallocation that respects the
    semantics of the arbitrary‑precision seq_t type. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* utility for safe reallocation of seq_t arrays
   (handles copy/move semantics of the seq_t type). */
#include "../utils/seq_realloc.h"

/* seq_t and idx_t are provided by the build system */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/*   Dynamic array helpers for seq_t                                 */
/* ------------------------------------------------------------------ */

static inline void grow_seq(seq_t **arr, size_t *cap, idx_t needed)
{
    if (*cap >= (size_t)needed)
        return;
    size_t new_cap = (*cap) ? *cap : 1;
    while (new_cap < (size_t)needed)
        new_cap <<= 1;
    *arr = seq_realloc(*arr, *cap, new_cap);
    *cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*   Memoised storage for coefficients q(k) (Pi(1‑x^k))              */
/*   and for the target sequence a(n).                                 */
/* ------------------------------------------------------------------ */

static seq_t *q_arr = NULL;           /* q[0] = 1 */
static size_t   q_cap = 0;
static idx_t    q_computed = 0;        /* largest q index computed */

static seq_t *a_arr = NULL;           /* a[0] = 1 */
static size_t   a_cap = 0;
static idx_t    a_computed = 0;        /* largest a index computed */

/* ------------------------------------------------------------------ */
/*   Main function                                                    */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* ensure arrays large enough */
    grow_seq(&q_arr, &q_cap, n + 1);
    grow_seq(&a_arr, &a_cap, n + 1);

    /* initialise zeroth entries once */
    if (q_computed == 0 && a_computed == 0) {
        q_arr[0] = (seq_t)1;
        a_arr[0] = (seq_t)1;
    }

    /* -------------------------------------------------------------- */
    /*   compute coefficients q(k) up to n                               */
    /* -------------------------------------------------------------- */
    for (idx_t i = q_computed + 1; i <= n; ++i) {
        seq_t val = 0;
        for (idx_t m = 1; ; ++m) {
            idx_t g1 = m * (3 * m - 1) / 2;
            if (g1 > i) break;
            if (g1 == i)
                val += (m % 2 == 0) ? (seq_t)1 : (seq_t)-1;

            idx_t g2 = m * (3 * m + 1) / 2;
            if (g2 > i) continue;
            if (g2 == i)
                val += (m % 2 == 0) ? (seq_t)1 : (seq_t)-1;
        }
        q_arr[i] = val;
    }
    if (q_computed < n)
        q_computed = n;

    /* -------------------------------------------------------------- */
    /*   compute a(n) values up to n                                 */
    /* -------------------------------------------------------------- */
    for (idx_t i = a_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += q_arr[k] * a_arr[i - k];
        }
        a_arr[i] = sum;
    }
    if (a_computed < n)
        a_computed = n;

    return a_arr[n];
}
