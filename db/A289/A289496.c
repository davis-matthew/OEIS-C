/*
 * A289496  Number of primes in the interval [5n, 6n].
 * https://oeis.org/A289496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = π(6n) – π(5n‑1) where π(x) is the prime counting
 *            function. The function builds a sieve of Eratosthenes up to the
 *            maximum required limit on demand and keeps a prefix array of
 *            prime counts so each query runs in O(1)
 * Range:     u8 n <= 2427, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static idx_t _sieve_limit = 0;
static bool   *_sieve_is_prime = NULL;
static seq_t  *_sieve_pi      = NULL;

/* Expand the sieve and prefix array up to at least 'limit'.
   New memory replaces old one. */
static void _ensure_sieve(idx_t limit)
{
    if (limit <= _sieve_limit) return;

    /* Resize arrays */
    bool *new_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    seq_t *new_pi   = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!new_prime || !new_pi) return;   /* allocation failure; caller checks */

    memset(new_prime, 1, (limit + 1) * sizeof(bool));
    new_prime[0] = new_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (new_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_prime[j] = false;
        }
    }

    new_pi[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        new_pi[i] = new_pi[i - 1] + (new_prime[i] ? 1 : 0);

    /* Replace old arrays */
    free(_sieve_is_prime);
    free(_sieve_pi);
    _sieve_is_prime = new_prime;
    _sieve_pi       = new_pi;
    _sieve_limit    = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* out of range – safe guard */

    idx_t lo = 5 * n;
    idx_t hi = 6 * n;
    _ensure_sieve(hi);

    /* 5n-1 is safe because n>=1 -> lo>=5 */
    seq_t count_hi = _sieve_pi[hi];
    seq_t count_lo = _sieve_pi[lo - 1];
    return count_hi - count_lo;
}
