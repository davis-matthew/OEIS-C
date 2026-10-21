/*
 * A248687  Sum of the numbers in row n of the triangular array at A248686.
 * https://oeis.org/A248687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} n!/(n(1)!*...*n(k)!) where n(i) =
 *            floor((n+i-1)/k) This implementation precomputes factorials up
 *            to n and evaluates each term as n! / ( fact[q]^(k-r) *
 *            fact[q+1]^r ), with q = n/k and r = n%k. The power function uses
 *            exponentiation by squaring on seq_t
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   pow_seq_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_seq_simple.h"

/* static cache for factorials */
static seq_t *fact_cache = NULL;
static idx_t fact_cap = 0;   /* number of factorials stored (fact_cap-1 is last index) */

/* ensure fact_cache contains factorials up to n (inclusive) */
static void ensure_fact_cache(idx_t n)
{
    if (n + 1 <= fact_cap)          /* already enough */
        return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(fact_cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – abort the program (should not happen in tests) */
        abort();
    }
    fact_cache = tmp;
    if (fact_cap == 0) {
        fact_cache[0] = (seq_t)1;
        fact_cap = 1;
    }
    /* compute new factorials */
    for (idx_t i = fact_cap; i <= n; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_cap = new_cap;
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* not defined for n < 1, return 0 */

    /* precompute factorials up to n */
    ensure_fact_cache(n);
    seq_t fact_n = fact_cache[n];
    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;     /* floor(n/k) */
        idx_t r = n % k;     /* remainder, number of parts of size q+1 */

        /* compute product = fact[q]^(k-r) * fact[q+1]^r */
        seq_t prod_q   = pow_seq_simple(fact_cache[q], (int)(k - r));
        seq_t prod_qp1 = (r == 0) ? (seq_t)1
                                 : pow_seq_simple(fact_cache[q + 1], (int)r);
        seq_t prod = prod_q * prod_qp1;

        /* term = n! / prod */
        seq_t term = fact_n / prod;
        sum += term;
    }
    return sum;
}
