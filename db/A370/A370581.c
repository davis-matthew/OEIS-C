/*
 * A370581  a(n) = n! * Sum_{d|n} d/(d! * (n/d)!).
 * https://oeis.org/A370581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 26,
 *            big any n
 * Helpers:   _exp_fact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A370581:
 *   a(n) = n! * Sum_{d|n} d / (d! * (n/d)!)
 *
 * Implementation details:
 *   • The sum runs over all divisors d of n.
 *   • For each term we avoid a big factorial by using
 *     prime‑factor exponents:
 *
 *         term(d) = d *   Π_p  p^{ e(n,p) – e(d,p) – e(m,p) }
 *
 *     where m = n/d and e(x,p) is the exponent of prime p in x!.
 *   • The primes up to the current n are generated once by a
 *     simple sieve.  The exponent e(x,p) is computed by
 *     repeated division.
 *
 *   The algorithm runs in O(#divisors(n) * #primes(n) * log_p n)
 *   which is well below the required limits for n ≤ 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_exp_fact.h"

/* ------------------------------------------------------------------ */
/* Prime sieve cached for the maximum n requested so far.             */
/* ------------------------------------------------------------------ */
static idx_t _prime_limit = 0;
static int   *_prime_list  = NULL;
static size_t _prime_cnt   = 0;

/* Compute primes up to 'limit' (inclusive) using a simple sieve.      */
static void _init_primes(idx_t limit) {
    if (_prime_limit >= limit) return;
    /* Allocate boolean array for sieve. */
    char *is_prime = (char *)malloc((size_t)(limit + 1));
    if (!is_prime) return; /* out of memory, unlikely in tests */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Count primes. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    /* Allocate prime list. */
    int *list = (int *)malloc(cnt * sizeof(int));
    if (!list) { free(is_prime); return; }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) list[idx++] = (int)i;

    free(is_prime);
    /* Resize primes array if needed. */
    if (_prime_list) free(_prime_list);
    _prime_list = list;
    _prime_cnt  = cnt;
    _prime_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;    /* safety, offset is 1 */

    /* Prepare primes up to n. */
    _init_primes(n);

    seq_t total = 0;

    /* Enumerate all divisors of n. */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;
        idx_t m1 = d;
        idx_t m2 = n / d;

        /* Process the divisor d. */
        seq_t term = (seq_t)m1;          /* start with the multiplier d   */
        for (size_t pi = 0; pi < _prime_cnt && _prime_list[pi] <= n; ++pi) {
            int p = _prime_list[pi];
            long long exp = _exp_fact(n, p)
                         - _exp_fact(m1, p)
                         - _exp_fact(m2, p);
            if (exp <= 0) continue;
            /* Multiply term by p^exp. */
            for (long long k = 0; k < exp; ++k)
                term *= (seq_t)p;
        }
        total += term;

        /* If d != m, process the complementary divisor m. */
        if (m1 != m2) {
            term = (seq_t)m2;          /* start with the multiplier m   */
            idx_t d2 = m2;
            idx_t m22 = n / d2;         /* which is d1 */
            for (size_t pi = 0; pi < _prime_cnt && _prime_list[pi] <= n; ++pi) {
                int p = _prime_list[pi];
                long long exp = _exp_fact(n, p)
                             - _exp_fact(d2, p)
                             - _exp_fact(m22, p);
                if (exp <= 0) continue;
                for (long long k = 0; k < exp; ++k)
                    term *= (seq_t)p;
            }
            total += term;
        }
    }
    return total;
}
