/*
 * A322006  a(n) = number of primes of the form p = n - q, where q is a prime
 *          or semiprime.
 * https://oeis.org/A322006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 1679, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes a(n) for OEIS A322006:
 *   a(n) = number of primes p such that p = n − q,
 *   where q is either a prime or a semiprime (product of two primes).
 *
 * Implementation details:
 *   * For each call, perform a sieve of Eratosthenes up to n.
 *   * Store all primes ≤ n in a small array for fast iteration.
 *   * For every prime p ≤ n, compute q = n − p.
 *   * Check q by:
 *        – If q is prime (look‑up in the sieve array), count it.
 *        – Otherwise, test whether q is a product of two primes
 *          by dividing by each stored prime ≤ √q.
 *
 * The algorithm runs in O(n log log n + π(n)·√n) per call,
 * which is far below the 2‑second limit for n ≤ 10000.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build system; no definitions needed. */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* Sieve of Eratosthenes up to n. */
    size_t sz = (size_t)n + 1;
    uint8_t *is_prime = (uint8_t *)calloc(sz, sizeof(uint8_t));
    if (!is_prime)
        return (seq_t)0;            /* memory allocation failed */

    /* Mark all numbers ≥2 initially as prime, then sieve. */
    for (idx_t i = 2; i <= n; ++i)
        is_prime[(size_t)i] = 1;
    for (idx_t p = 2; (idx_t)(p * p) <= n; ++p) {
        if (is_prime[(size_t)p]) {
            for (idx_t k = p * p; k <= n; k += p)
                is_prime[(size_t)k] = 0;
        }
    }

    /* Count primes to allocate an array of them. */
    size_t prime_count = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[(size_t)i])
            ++prime_count;

    uint64_t *primes = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return (seq_t)0;
    }

    /* Fill the primes array. */
    size_t pidx = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[(size_t)i])
            primes[pidx++] = (uint64_t)i;

    /* Main counting loop. */
    seq_t count = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        uint64_t q = (uint64_t)n - p;
        if (q < 2)
            continue;

        /* Quick prime test using sieve array. */
        if (is_prime[(size_t)q]) {
            ++count;
            continue;
        }

        /* Semiprime test: try to divide q by each stored prime ≤ √q. */
        for (size_t j = 0; j < prime_count && primes[j] * primes[j] <= q; ++j) {
            uint64_t r = primes[j];
            if (q % r == 0) {
                uint64_t s = q / r;
                if (is_prime[(size_t)s]) {
                    ++count;
                    break;
                }
            }
        }
    }

    free(primes);
    free(is_prime);
    return count;
}
