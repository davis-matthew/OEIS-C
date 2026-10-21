/*
 * A280981  Partial products of A049820; a(1) = a(2) = 1.
 * https://oeis.org/A280981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=3..n} (i - τ(i)) , a(1) = a(2) = 1 where τ(m) is the
 *            number of positive divisors of m
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 16, u64 n <= 24, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build. */
static idx_t *divcnt = NULL;     /* divisor counts: τ(i) */
static idx_t divcnt_max = 0;     /* maximum index computed */

/* Ensure divisor counts up to limit are available. */
static void ensure_divcnt(idx_t limit) {
    if (limit <= divcnt_max) return;

    /* Allocate new array and recompute divisor counts up to limit. */
    if (divcnt != NULL) free(divcnt);
    divcnt = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (divcnt == NULL) return;   /* allocation failed; caller will misbehave */

    /* Initialize to 0 and then use a simple sieve to count divisors. */
    for (idx_t i = 0; i <= limit; ++i) divcnt[i] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i) {
            divcnt[j]++;            /* j has one more divisor: i */
        }
    }
    divcnt_max = limit;
}

seq_t a(idx_t n) {
    if (n < 3) return (seq_t)1;    /* a(1) = a(2) = 1 */

    ensure_divcnt(n);              /* make τ(i) available for i <= n */

    seq_t result = (seq_t)1;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t term = (seq_t)i - (seq_t)divcnt[i];
        result *= term;            /* multiply term into the running product */
    }
    return result;
}
