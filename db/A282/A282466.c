/*
 * A282466  a(n) = n*a(n-1) + n!, with n>0, a(0)=5.
 * https://oeis.org/A282466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 5) * n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of factorials: fact[i] = i! for i <= fact_len */
static seq_t *fact = NULL;
static idx_t fact_len = 0;

/* Ensure factorials up to n are computed and stored in fact[]. */
static void ensure_factorial(idx_t n) {
    if (fact == NULL) {
        fact = (seq_t*)calloc(1, sizeof(seq_t));
        fact[0] = (seq_t)1;
        fact_len = 0;
    }
    if (n <= fact_len)
        return;

    idx_t old_len = fact_len;
    idx_t new_len = n;
    seq_t *tmp = (seq_t*)realloc(fact, (new_len + 1) * sizeof(seq_t));
    fact = tmp;
    for (idx_t i = old_len + 1; i <= new_len; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_len = new_len;
}

/* Sequence a(n) = (n + 5) * n! for n >= 0 */
seq_t a(idx_t n) {
    ensure_factorial(n);
    return (seq_t)(n + 5) * fact[n];
}
