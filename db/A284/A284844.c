/*
 * A284844  Number of permutations on [n+3] with no circular 3-successions.
 * https://oeis.org/A284844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+3) * Sum_{j=0..n} (-1)^j * binomial(n,j) * (n-j+2)!
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* Cache factorials up to (n+2)!.
       Largest n we expect is 10000, so we allocate a fixed array of size 10003
       (indices 0 ... 10002). */
    static seq_t fact[10005];
    static idx_t fact_cache = 0;          /* largest factorial index already computed */
    static bool initialized = false;      /* true when fact[0] is set */

    if (!initialized) {
        fact[0] = (seq_t)1;
        initialized = true;
    }

    /* Ensure the factorial array contains up to (n+2)! */
    idx_t required = n + 2;
    if (fact_cache < required) {
        for (idx_t i = fact_cache + 1; i <= required; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_cache = required;
    }

    seq_t sum_even = 0;
    seq_t sum_odd  = 0;
    seq_t binom    = (seq_t)1;  /* C(n,0) */

    for (idx_t j = 0; j <= n; ++j) {
        idx_t m = n - j + 2;          /* factorial index for (n-j+2)! */
        seq_t term = binom * fact[m];

        if (j & 1)
            sum_odd += term;
        else
            sum_even += term;

        if (j < n) {
            /* Update binomial coefficient: C(n, j+1) = C(n, j) * (n-j) / (j+1) */
            binom = binom * (seq_t)(n - j) / (seq_t)(j + 1);
        }
    }

    seq_t diff  = sum_even - sum_odd;          /* sum_{even} - sum_{odd}  */
    seq_t result = (seq_t)(n + 3) * diff;      /* (n+3) * (Resulting sum) */

    return result;
}
