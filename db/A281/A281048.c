/*
 * A281048  Expansion of x*(1 - x)*Product_{k>=0} (1 + x^(2^k) - x^(2^(k+1))).
 * https://oeis.org/A281048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^(n-1) in the series x*(1 -
 *            x)*Product_{k>=0}(1 + x^(2^k) - x^(2^(k+1)))
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation keeps the product polynomial P(x) =
   ∏(1 + x^(2^k) - x^(2^(k+1))) incrementally.  For each n the
   coefficient a(n) is computed as P[n-1] - P[n-2] (with P[-1] treated
   as 0).  This evaluates the generating function directly in O(n)
   time overall.  */

#include <stdlib.h>
#include <stdint.h>

/* The following static helpers maintain the product polynomial. */
static seq_t *P = NULL;          /* coefficients of the product polynomial */
static size_t P_cap = 0;         /* allocated array size */
static size_t computed_high = 0; /* highest degree already computed */
static size_t next_k = 0;        /* next exponent index to process (k) */

/* Allocate enough room for indices up to min_cap-1.  Newly allocated
   entries are initialized to zero.  The realloc result is cast to
   seq_t* to keep the code C++‑complaint. */
static void ensure_capacity(size_t min_cap) {
    if (P_cap >= min_cap) return;
    size_t old_cap = P_cap;
    size_t new_cap = P_cap ? P_cap : 1;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(P, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    /* Zero the newly allocated elements */
    for (size_t i = old_cap; i < new_cap; ++i) tmp[i] = 0;
    P = tmp;
    P_cap = new_cap;
}

/* Multiply the current polynomial P(x) (degree computed_high) by the
   factor (1 + x^a - x^b) where a=2^k, b=2^(k+1).  The result is
   stored back in P.  The update scans from high to low to preserve
   old coefficients while they are used. */
static void multiply_factor(size_t a, size_t b, size_t old_high) {
    size_t new_high = old_high + b;
    ensure_capacity(new_high + 1);

    for (size_t i = new_high; ; --i) {
        seq_t val = (i <= old_high) ? P[i] : 0;
        if (i >= a) val += P[i - a];
        if (i >= b) val -= P[i - b];
        P[i] = val;
        if (i == 0) break;
    }
    computed_high = new_high;
}

/* Ensure that all polynomial terms up to degree "target" are
   available.  If the product hasn't been initialized yet it is
   started with P[0] = 1. */
static void extend_to_target(size_t target) {
    if (!P) {
        ensure_capacity(1);
        P[0] = 0;    /* zero-initialized in ensure_capacity */
        P[0] = 1;    /* the product at start is 1 */
        computed_high = 0;
    }
    while ((1ULL << next_k) <= target) {
        size_t a = 1ULL << next_k;   /* 2^k   */
        size_t b = a << 1;           /* 2^(k+1) */
        multiply_factor(a, b, computed_high);
        ++next_k;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* defensive, though n >= 1 by OEIS */

    /* The product polynomial must contain terms up to degree n-1. */
    size_t target = (size_t)(n - 1);
    extend_to_target(target);

    /* a(n) = coefficient of x^(n-1) in x*(1-x)*P(x) = P[n-1] - P[n-2] */
    seq_t term = P[n - 1];
    if (n >= 2) term -= P[n - 2];
    return term;
}
