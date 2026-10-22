/*
 * A354888  a(n) = n! * Sum_{d|n} d^d / d!.
 * https://oeis.org/A354888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} d^d / d! (n >= 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, return 0 */

    /* Static factorial array up to the current maximum n */
    static seq_t *fact = NULL;
    static idx_t fact_capacity = 0;

    if (fact == NULL) {               /* first call: initialise */
        fact_capacity = 1;
        fact = (seq_t*)malloc(sizeof(seq_t));
        fact[0] = (seq_t)1;
    }

    /* Grow and recompute factorial values if needed */
    if (n + 1 > fact_capacity) {
        idx_t old_cap = fact_capacity;
        fact_capacity = n + 1;
        seq_t *new_fact = (seq_t*)malloc(fact_capacity * sizeof(seq_t));
        for (idx_t i = 0; i < old_cap; ++i)
            new_fact[i] = fact[i];
        for (idx_t i = old_cap; i <= n; ++i)
            new_fact[i] = new_fact[i-1] * (seq_t)i;
        free(fact);
        fact = new_fact;
    }

    seq_t fact_n = fact[n];
    seq_t result = (seq_t)0;

    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            seq_t term = (fact_n / fact[d]) * big_pow((seq_t)d, (seq_t)d);
            result += term;
        }
    }

    return result;
}
