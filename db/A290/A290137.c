/*
 * A290137  Number of compositions (ordered partitions) of n into nonprime
 *          squarefree parts (A000469).
 * https://oeis.org/A290137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of compositions of n into non‑prime squarefree parts
 *            (OEIS A290137). Recurrence: a(0)=1, a(n)=∑_{k∈S, k≤n} a(n−k),
 *            where S={m≥1 | m is squarefree and not prime}
 * Range:     u8 n <= 22, u16 n <= 41, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset */
    if (n == (idx_t)0) return (seq_t)1;

    /* Dynamic programming table (memoized) */
    static seq_t *memo = NULL;
    static size_t memo_cap = 0;
    if (memo_cap < (size_t)(n + 1)) {
        seq_t *new_mem = (seq_t *)realloc(memo, (n + 1) * sizeof(seq_t));
        if (!new_mem) return (seq_t)0;            /* allocation failure */
        memo = new_mem;
        memo_cap = (size_t)(n + 1);
    }
    /* Re‑compute from scratch for the requested n */
    for (size_t i = 0; i <= (size_t)n; ++i) memo[i] = (seq_t)0;
    memo[0] = (seq_t)1;

    /* Sieve for primes up to n */
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    if (!is_prime) return (seq_t)0;
    for (idx_t i = 0; i <= n; ++i) is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;
    for (idx_t p = 2; p <= n / p; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= n; j += p) is_prime[j] = false;
        }
    }

    /* Mark squarefree numbers up to n */
    bool *is_sq = (bool *)malloc((n + 1) * sizeof(bool));
    if (!is_sq) {
        free(is_prime);
        return (seq_t)0;
    }
    for (idx_t i = 0; i <= n; ++i) is_sq[i] = true;
    if (n >= 0) is_sq[0] = false;
    for (idx_t p = 2; p <= n / p; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= n; j += p * p) is_sq[j] = false;
        }
    }

    /* Build the list of allowed parts (squarefree and not prime) */
    idx_t *allowed = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!allowed) {
        free(is_prime);
        free(is_sq);
        return (seq_t)0;
    }
    size_t allow_cnt = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (is_sq[i] && !is_prime[i]) {
            allowed[allow_cnt++] = i;
        }
    }

    /* Dynamic programming over compositions */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t k = 0; k < allow_cnt && allowed[k] <= i; ++k) {
            sum += memo[i - allowed[k]];
        }
        memo[i] = sum;
    }

    /* Clean up */
    free(is_prime);
    free(is_sq);
    free(allowed);

    return memo[n];
}
