/*
 * A379306  Number of squarefree prime indices of n.
 * https://oeis.org/A379306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of squarefree prime indices of n * A prime index
 *            of n is the position of a prime that divides n. * The sequence
 *            counts, with multiplicity, each such index that is squarefree
 * Range:     u8 n <= 7078, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_squarefree, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/sieve_primes.h"
#include "../utils/is_squarefree.h"
#include "../utils/is_prime.h"

static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_initialized = false;
static const idx_t PRIME_LIMIT = 100000;   /* sieve range – enough for n ≤ 10 000 */

static void init_primes(void) {
    if (primes_initialized) return;
    sieve_primes(PRIME_LIMIT, &primes, &primes_cnt);
    primes_initialized = true;
}

/* return the 1‑based index of prime p in the list of primes */
static idx_t prime_index(idx_t p) {
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] < p) lo = mid + 1;
        else if (primes[mid] > p) hi = mid;
        else return (idx_t)(mid + 1);   /* found, indices are 1‑based */
    }
    /* p not in the precomputed table – count primes up to p on the fly */
    idx_t m = (idx_t)primes_cnt + 1;
    for (idx_t q = PRIME_LIMIT + 1; q <= p; ++q) {
        if (is_prime(q)) ++m;
    }
    return m;
}

seq_t a(idx_t n) {
    init_primes();
    if (n <= 1) return 0;
    idx_t tmp = n;
    seq_t result = 0;

    /* trial division using the precomputed primes */
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        idx_t p = primes[i];
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            idx_t idx = prime_index(p);
            if (is_squarefree((seq_t)idx)) {
                result += (seq_t)exp;
            }
        }
    }

    /* remaining prime factor (if any) */
    if (tmp > 1) {
        idx_t idx = prime_index(tmp);
        if (is_squarefree((seq_t)idx)) {
            result += 1;   /* exponent is 1 */
        }
    }

    return result;
}
