/*
 * A366368  a(n) = LCM of pairwise products of distinct integers from
 *          {1,2,...,n}.
 * https://oeis.org/A366368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM of pairwise products of distinct integers from
 *            {1,2,...,n} Equivalent to a(n) = A003418(n) *
 *            A003418(floor(n/2))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses product of lcm(1..n) and lcm(1..floor(n/2)) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"          /* seq_mul(seq_t, seq_t) */
#include "../utils/seq_pow.h"          /* seq_pow(seq_t, idx_t) */
#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t, uint64_t**, size_t*) */

static uint64_t *cached_primes = NULL;   /* array of primes up to cached_limit */
static size_t cached_prime_cnt   = 0;     /* number of primes in cached_primes */
static idx_t   cached_prime_limit = 0;    /* maximum n for which primes are cached */

/* Ensure we have all primes <= n in cached_primes */
static void ensure_primes_up_to(idx_t n)
{
    if (n <= cached_prime_limit) return;

    /* Free previous array if any */
    if (cached_primes != NULL) {
        free(cached_primes);
        cached_primes = NULL;
    }

    build_primes_upto(n, &cached_primes, &cached_prime_cnt);
    cached_prime_limit = n;
}

/* Compute LCM(1..m) = ∏_{p≤m} p^{⌊log_p m⌋} */
static seq_t lcm_upto(idx_t m)
{
    if (m < 2) return (seq_t)1;   /* LCM of empty set is defined as 1 */

    ensure_primes_up_to(m);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cached_prime_cnt; ++i) {
        uint64_t p = cached_primes[i];
        if ((idx_t)p > m) break;      /* primes are sorted ascending */

        idx_t e = 0;
        idx_t t = m;
        while (t >= p) {               /* count ⌊log_p m⌋ */
            t /= p;
            ++e;
        }
        if (e > 0) {
            seq_t pow = seq_pow((seq_t)p, e);
            result = seq_mul(result, pow);
        }
    }
    return result;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    seq_t lcm_n     = lcm_upto(n);
    seq_t lcm_half  = lcm_upto(n >> 1);   /* floor(n/2) */
    return seq_mul(lcm_n, lcm_half);
}
