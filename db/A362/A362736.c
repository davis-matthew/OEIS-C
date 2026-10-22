/*
 * A362736  E.g.f. satisfies A(x) = exp(x^2 + x / A(x)).
 * https://oeis.org/A362736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A362736 from OEIS
 * Formula: a(n) = n! * Sum_{k=0..floor(n/2)} (-n+2*k+1)^(n-k-1) / (k! * (n-2*k)!)
 *
 * The function uses exact integer arithmetic only.  A table of factorials
 * is built lazily up to the required index.
 */
#include <stdint.h>
#include <stdlib.h>

/* Local helper: fast exponentiation of integer bases */
static seq_t pow_big(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t cur = base;
    while (exp) {
        if (exp & 1)
            result = result * cur;
        cur = cur * cur;
        exp >>= 1;
    }
    return result;
}

/* Lazy factorial cache */
static seq_t *fact_cache = NULL;   /* array of factorial values   */
static idx_t fact_size = 0;        /* number of elements allocated */

static void ensure_factorial(idx_t n) {
    /* enlarge cache if necessary */
    if (n < fact_size)
        return;
    size_t old_size = fact_size;
    size_t new_size = (size_t)(n + 1);
    fact_cache = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
    if (old_size == 0) {
        fact_cache[0] = 1;
        old_size = 1;
    }
    for (size_t i = old_size; i <= (size_t)n; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)(i);
    }
    fact_size = new_size;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 1;          /* base case */
    ensure_factorial(n);

    seq_t total = 0;
    idx_t half = n >> 1;           /* floor(n/2) */

    /* iterate over k */
    for (idx_t k = 0; k <= half; ++k) {
        /* coefficient: n! / (k! * (n-2k)!) */
        seq_t coeff = fact_cache[n];
        coeff /= fact_cache[k];
        coeff /= fact_cache[n - 2 * k];

        /* exponent */
        idx_t exp = n - k - 1;

        /* base: 2k - n + 1 */
        seq_t base = (seq_t)(2 * k) - (seq_t)n + 1;

        /* power of the base */
        seq_t pow = pow_big(base, exp);

        /* add contribution */
        total += coeff * pow;
    }

    return total;
}
