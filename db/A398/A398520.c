/*
 * A398520  a(n) = Sum_{k=1..n} tau(floor(n/k)), where tau(n) = A000005(n) is
 *          the number of divisors of n.
 * https://oeis.org/A398520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} tau(floor(n/k))
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute divisor counts up to n, reuse cache when possible */
static seq_t* tau_cache = NULL;
static idx_t tau_cap = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Resize divisor array if needed */
    if (n >= tau_cap) {
        idx_t new_cap = n + 1;  /* include index 0 */
        seq_t *tmp = (seq_t *)realloc(tau_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed */
        tau_cache = tmp;
        tau_cap = new_cap;
    }

    /* Compute divisor counts tau[1..n] */
    for (idx_t i = 1; i <= n; ++i) {
        tau_cache[i] = 0;
    }
    for (idx_t d = 1; d <= n; ++d) {
        for (idx_t j = d; j <= n; j += d) {
            ++tau_cache[j];
        }
    }

    /* Sum tau(floor(n/k)) using grouping trick */
    seq_t sum = 0;
    idx_t i = 1;
    while (i <= n) {
        idx_t v = n / i;
        idx_t last = n / v;
        seq_t cnt = (seq_t)(last - i + 1);
        sum += cnt * tau_cache[v];
        i = last + 1;
    }
    return sum;
}
