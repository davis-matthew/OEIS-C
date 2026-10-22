/*
 * A344982  Number of Goldbach partitions of 2n into two odd-indexed primes.
 * https://oeis.org/A344982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Goldbach partitions of 2n into two odd‑indexed
 *            primes
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static scratch space used by all calls of a() */
static bool *prime_flag = NULL;
static idx_t *pi_count = NULL;
static idx_t current_limit = 0;

/* build or extend the sieve up to at least 'limit' */
static void ensure_sieve(idx_t limit)
{
    if (limit <= current_limit)
        return;

    /* allocate new arrays */
    bool *new_flag = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_flag)
        return;
    idx_t *new_pi = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!new_pi) {
        free(new_flag);
        return;
    }

    /* initialise and run sieve */
    for (idx_t i = 0; i <= limit; i++)
        new_flag[i] = true;
    new_flag[0] = new_flag[1] = false;

    for (idx_t p = 2; p * p <= limit; p++) {
        if (new_flag[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                new_flag[j] = false;
        }
    }

    /* build prefix (pi) array */
    idx_t cnt = 0;
    for (idx_t i = 0; i <= limit; i++) {
        if (new_flag[i])
            cnt++;
        new_pi[i] = cnt;
    }

    /* drop old storage */
    free(prime_flag);
    free(pi_count);
    prime_flag = new_flag;
    pi_count   = new_pi;
    current_limit = limit;
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t limit = 2 * n;
    ensure_sieve(limit);

    seq_t count = 0;
    for (idx_t k = 1; k <= n; k++) {
        idx_t l = limit - k;
        if (!prime_flag[k] || !prime_flag[l])
            continue;
        if ((pi_count[k] & 1u) && (pi_count[l] & 1u))
            count++;
    }
    return count;
}
