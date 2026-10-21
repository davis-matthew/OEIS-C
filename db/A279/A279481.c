/*
 * A279481  Count the primes appearing in each interval [p,q] where (p,q) is a
 *          Goldbach partition of 2n, and then add the results.
 * https://oeis.org/A279481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=3..n} A010051(i) * A010051(2*n-i) * (pi(2*n-i) -
 *            pi(i-1)), where A010051(x) = 1 iff x is an odd prime. Prime 2 is
 *            excluded from all Goldbach partitions, matching the OEIS terms
 *            (a(2)=0)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define MAX_LIMIT 20000          /* sufficient for n <= 10000 (2*n <= 20000) */

static bool _prime_flag[MAX_LIMIT + 1];
static idx_t _pi_prefix[MAX_LIMIT + 1];
static bool _sieve_done = false;

static void _init_sieve(void)
{
    /* Sieve of Eratosthenes */
    for (idx_t i = 0; i <= MAX_LIMIT; ++i)
        _prime_flag[i] = true;
    _prime_flag[0] = _prime_flag[1] = false;
    for (idx_t p = 2; p * p <= MAX_LIMIT; ++p) {
        if (_prime_flag[p]) {
            for (idx_t m = p * p; m <= MAX_LIMIT; m += p)
                _prime_flag[m] = false;
        }
    }
    /* Prefix counts of primes up to each index */
    _pi_prefix[0] = 0;
    for (idx_t i = 1; i <= MAX_LIMIT; ++i)
        _pi_prefix[i] = _pi_prefix[i - 1] + (_prime_flag[i] ? 1 : 0);
    _sieve_done = true;
}

seq_t a(idx_t n)
{
    if (!_sieve_done)
        _init_sieve();

    if (n < 2)      /* a(1) = 0 by definition; a(0) irrelevant */
        return 0;

    seq_t sum = 0;
    idx_t two_n = n + n;          /* 2*n */

    for (idx_t i = 3; i <= n; ++i) {           /* start at 3 to exclude prime 2 */
        if (!_prime_flag[i] || !_prime_flag[two_n - i])
            continue;
        /* primes in [i, 2*n-i] inclusive */
        seq_t primes_in_interval = (seq_t)(_pi_prefix[two_n - i] - _pi_prefix[i - 1]);
        sum += primes_in_interval;
    }

    return sum;
}
