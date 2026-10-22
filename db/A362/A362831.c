/*
 * A362831  Number of partitions of n into two distinct parts (s,t) such that
 *          pi(s) = pi(t).
 * https://oeis.org/A362831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=1..⌊(n-1)/2⌋} [π(k) = π(n-k)]
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cached prime counting array.
   pi_array[i] = π(i) for all i ≤ pi_max.  */
static idx_t *pi_array   = NULL;
static idx_t  pi_max      = 0;

/* Rebuild the sieve and π-table up to (and including) n. */
static void _ensure_pi(idx_t n)
{
    if (n <= pi_max)
        return;

    size_t new_cap = (size_t)(n + 1);
    idx_t *tmp = (idx_t*)realloc(pi_array, new_cap * sizeof(idx_t));
    if (!tmp)
        abort();               /* allocation failure */
    pi_array = tmp;

    bool *is_prime = (bool*)calloc(new_cap, sizeof(bool));
    if (!is_prime)
        abort();               /* allocation failure */

    /* Mark all numbers >1 as potentially prime. */
    for (idx_t i = 2; i <= n; ++i)
        is_prime[i] = true;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= n; m += p)
                is_prime[m] = false;
        }
    }

    idx_t count = 0;
    pi_array[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (is_prime[i])
            ++count;
        pi_array[i] = count;
    }

    free(is_prime);
    pi_max = n;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n < 2)            /* a(1) = 0, values below the offset are irrelevant */
        return (seq_t)0;

    /* We may need π(k) for k ≤ n‑1. */
    _ensure_pi(n - 1);

    idx_t limit = (n - 1) / 2;
    idx_t cnt   = 0;

    for (idx_t k = 1; k <= limit; ++k) {
        if (pi_array[k] == pi_array[n - k])
            ++cnt;
    }

    return (seq_t)cnt;
}
