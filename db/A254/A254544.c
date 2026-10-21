/*
 * A254544  Number of length 6 1..(n+2) arrays with no leading partial sum
 *          equal to a prime.
 * https://oeis.org/A254544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP counting length‑6 arrays over 1..n+2 with no prefix sum equal
 *            to a prime
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 49, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but guard against underflow */

    /* Number of array values */
    idx_t m = n + 2;                      /* entries are in 1..m */
    idx_t max_sum = 6 * m;                /* maximum possible partial sum */

    /* Sieve for primes up to max_sum */
    bool *is_prime = (bool*)calloc((size_t)max_sum + 1, sizeof(bool));
    if (!is_prime) return (seq_t)0;        /* allocation failure */
    for (idx_t i = 2; i <= max_sum; ++i) is_prime[i] = true;
    for (idx_t p = 2; p * p <= max_sum; ++p)
        if (is_prime[p])
            for (idx_t q = p * p; q <= max_sum; q += p)
                is_prime[q] = false;

    /* DP arrays */
    seq_t *cnt     = (seq_t*)calloc((size_t)max_sum + 1, sizeof(seq_t));
    seq_t *new_cnt = (seq_t*)calloc((size_t)max_sum + 1, sizeof(seq_t));
    if (!cnt || !new_cnt) {
        free(is_prime);
        free(cnt); free(new_cnt);
        return (seq_t)0;
    }
    cnt[0] = (seq_t)1;          /* empty prefix sum */

    /* 6 steps */
    for (idx_t step = 1; step <= 6; ++step) {
        seq_t window = 0;       /* sliding window sum */

        for (idx_t s = 1; s <= max_sum; ++s) {
            /* extend window with cnt[s-1] */
            window += cnt[s - 1];

            /* remove element leaving the window */
            if (s > m) {
                window -= cnt[s - m - 1];
            }

            /* assign to new_cnt, zero if s is prime */
            new_cnt[s] = is_prime[s] ? (seq_t)0 : window;
        }

        /* swap pointers for next step and clear the buffer to be reused */
        seq_t *tmp = cnt;
        cnt = new_cnt;
        new_cnt = tmp;
        /* clear new_cnt for future use */
        for (idx_t i = 0; i <= max_sum; ++i) new_cnt[i] = (seq_t)0;
    }

    /* Sum all counts to obtain the total number of valid arrays */
    seq_t ans = (seq_t)0;
    for (idx_t s = 1; s <= max_sum; ++s) ans += cnt[s];

    /* Clean up */
    free(is_prime);
    free(cnt);
    free(new_cnt);

    return ans;
}
