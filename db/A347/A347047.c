/*
 * A347047  Smallest squarefree semiprime whose prime indices sum to n.
 * https://oeis.org/A347047
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest squarefree semiprime whose prime indices sum to
 *            n. Computed as a(n) = min_{i+j=n} p_i * p_j, where p_k is the
 *            k‑th prime
 * Range:     u8 n <= 14, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_vals = NULL;    /* prime_vals[0] = p1 = 2 */
static size_t prime_cnt = 0;

/* Estimate an upper bound for the nth prime using Rosser's theorem. */
static idx_t estimate_limit(idx_t n)
{
    if (n < 6) return 15;
    double nn = (double)n;
    double lim = nn * (log(nn) + log(log(nn))) + 3.0;
    return (idx_t)lim;
}

/* Ensure we have at least the first n primes in prime_vals. */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;  /* already enough */

    idx_t limit = estimate_limit(n);
    if (limit < 100) limit = 100;

    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);

    if (prime_vals) free(prime_vals);
    prime_vals = new_primes;
    prime_cnt = new_cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3) return 0;  /* undefined below the offset */

    ensure_primes(n);

    seq_t min_prod = 0;
    bool first = true;
    idx_t half = (n - 1) / 2;          /* i < j  ->  i <= floor((n-1)/2) */

    for (idx_t i = 1; i <= half; ++i) {
        idx_t j = n - i;
        seq_t prod = (seq_t)prime_vals[i - 1] * (seq_t)prime_vals[j - 1];
        if (first || prod < min_prod) {
            min_prod = prod;
            first = false;
        }
    }

    return min_prod;
}
