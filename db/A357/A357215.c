/*
 * A357215  a(n) = number of nonempty subsets S of {1, 2, ..., n} that contain
 *          only primes.
 * https://oeis.org/A357215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^π(n) – 1 where π(n) is the number of primes ≤ n
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   build_primes_upto, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow2.h"

static uint64_t *s_primes = NULL;     /* array of primes */
static size_t     s_prime_cnt = 0;     /* number of primes stored */
static idx_t      s_prime_limit = 0;   /* largest n for which primes are up to date */

/* Ensure that s_primes contains all primes ≤ n.  Resize the array if needed. */
static void
ensure_primes_upto(idx_t n)
{
    if (n <= s_prime_limit) return;

    if (s_primes) free(s_primes);

    size_t cnt;
    uint64_t *newp;
    build_primes_upto(n, &newp, &cnt);

    s_primes     = newp;
    s_prime_cnt  = cnt;
    s_prime_limit = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    ensure_primes_upto(n);

    /* 2^π(n) – 1, with π(n) = s_prime_cnt */
    seq_t res = pow2((idx_t)s_prime_cnt);
    res -= 1;
    return res;
}
