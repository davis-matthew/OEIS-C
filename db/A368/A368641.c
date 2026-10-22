/*
 * A368641  a(n) = Sum_{k=2..n} pi(k-1) * (ceiling(n/k) - floor(n/k)).
 * https://oeis.org/A368641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=2..n} π(k‑1) – Σ_{d|n,\;d≥2} π(d‑1) (i.e. sum of
 *            π(k‑1) for all k from 2 to n that do NOT divide n)
 * Range:     u8 n <= 40, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cached pi and prefix‑sum arrays */
static idx_t   cached_limit = 0u;
static idx_t  *pi_arr  = NULL;   /* pi values up to current limit (idx_t is fine) */
static seq_t  *sum_arr = NULL;   /* prefix sums of pi(k‑1) */

static void ensure_cache(idx_t n)
{
    if (n <= cached_limit) return;

    idx_t limit = n;
    unsigned char *is_prime = (unsigned char *)malloc((size_t)(limit + 1));
    if (!is_prime) exit(1);
    memset(is_prime, 1, (size_t)(limit + 1));

    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = 0;
        }
    }

    if (pi_arr)  free(pi_arr);
    if (sum_arr) free(sum_arr);

    pi_arr  = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    sum_arr = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!pi_arr || !sum_arr) exit(1);

    pi_arr[0]  = 0;
    pi_arr[1]  = 0;
    sum_arr[0] = 0;
    sum_arr[1] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        pi_arr[i]  = pi_arr[i - 1] + (idx_t)is_prime[i];
        sum_arr[i] = sum_arr[i - 1] + (seq_t)pi_arr[i - 1];
    }

    free(is_prime);
    cached_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1u) return (seq_t)0;   /* safety for unexpected input */

    ensure_cache(n);

    /* Total sum of π(k‑1) for k = 2..n */
    seq_t total = sum_arr[n];

    /* Subtract contributions of divisors of n (≥2) */
    seq_t divsum = 0;
    for (idx_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            divsum += (seq_t)pi_arr[d - 1];
            idx_t other = n / d;
            if (other != d && other >= 2)
                divsum += (seq_t)pi_arr[other - 1];
        }
    }
    /* Include divisor n itself (since other loop may skip it) */
    divsum += (seq_t)pi_arr[n - 1];

    return total - divsum;
}
