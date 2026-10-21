/*
 * A254532  Number of length n 1..(1+2) arrays with no leading partial sum
 *          equal to a prime.
 * https://oeis.org/A254532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n arrays with entries 1..3 such that no
 *            partial sum is prime
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 62, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Maximum possible partial sum after n steps */
    size_t max_sum = (size_t)3 * (size_t)n;

    /* DP arrays: cur holds counts for the current step,
       next holds counts for the next step. */
    seq_t *cur = (seq_t *)calloc(max_sum + 1, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(max_sum + 1, sizeof(seq_t));
    if (!cur || !next) { free(cur); free(next); return 0; }

    cur[0] = 1;  /* start with sum 0 */

    /* Sieve of Eratosthenes up to max_sum to identify prime sums. */
    unsigned char *is_prime = (unsigned char *)malloc(max_sum + 1);
    if (!is_prime) { free(cur); free(next); return 0; }
    for (size_t i = 0; i <= max_sum; ++i) is_prime[i] = 1;
    if (max_sum >= 0) is_prime[0] = 0;
    if (max_sum >= 1) is_prime[1] = 0;
    size_t limit = (size_t)(sqrt((double)max_sum) + 1.0);
    for (size_t p = 2; p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= max_sum; m += p) is_prime[m] = 0;
        }
    }

    /* Main DP loop over the array positions. */
    for (idx_t step = 1; step <= n; ++step) {
        size_t curr_max = (size_t)3 * (size_t)(step - 1);
        size_t next_max = (size_t)3 * (size_t)step;

        /* Zero out the next array up to next_max. */
        memset(next, 0, (next_max + 1) * sizeof(seq_t));

        for (size_t s = 0; s <= curr_max; ++s) {
            seq_t cnt = cur[s];
            if (cnt == 0) continue;
            /* Append 1, 2, or 3 if the new sum is not prime. */
            if (!is_prime[s + 1]) next[s + 1] += cnt;
            if (!is_prime[s + 2]) next[s + 2] += cnt;
            if (!is_prime[s + 3]) next[s + 3] += cnt;
        }

        /* Prepare for next iteration. */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum over all final sums. */
    seq_t total = 0;
    for (size_t s = 0; s <= max_sum; ++s) total += cur[s];

    /* Clean up. */
    free(cur);
    free(next);
    free(is_prime);

    return total;
}
