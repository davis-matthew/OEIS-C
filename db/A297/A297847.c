/*
 * A297847  Sexiness of p = prime(n): number of iterations of the function
 *          f(x) = x + 6 that leave p prime.
 * https://oeis.org/A297847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sexiness sequence:
   a(n) = number of successive applications of f(x) = x + 6 that
   keep the successive values prime, starting from the n‑th prime. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes.h"
#include "../utils/is_prime_big.h"

/* Cached list of primes up to the current limit. */
static idx_t *primes = NULL;
static size_t primes_cnt = 0;

/* Ensure that the cached array contains at least n primes. */
static void
ensure_nth_prime(idx_t n)
{
    if (primes_cnt >= n)
        return;                 /* already enough primes */

    free(primes);                /* drop old list */
    primes_cnt = 0;
    primes = NULL;

    /* _prime_limit_for_idx guarantees that the n-th prime is ≤ limit. */
    idx_t limit = _prime_limit_for_idx(n);
    build_primes(limit, &primes, &primes_cnt);
    /* primes_cnt will be ≥ n by the definition of _prime_limit_for_idx. */
}

/* Return the sexiness of the n-th prime. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1; handle out‑of‑range safely */

    /* Obtain prime(n). */
    ensure_nth_prime(n);
    seq_t p = (seq_t)primes[n - 1];

    /* Count how many successive +6 steps remain prime. */
    seq_t iter = 0;
    seq_t curr = p;
    while (true) {
        seq_t next = curr + (seq_t)6;
        if (is_prime_big(next)) {
            iter++;
            curr = next;
        } else {
            break;
        }
    }
    return iter;
}
