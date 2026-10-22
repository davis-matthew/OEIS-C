/*
 * A389978  Arithmetic mean of A367866(n), A389977(n), A367865(n), and
 *          A389775(n).
 * https://oeis.org/A389978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A367866(n)+A389977(n)+A367865(n)+A389775(n))/4 which
 *            reduces to the following evaluation for square‑free numbers
 *            (a(n) = a(rad(n))): a(n) = (1/4) * \sum_{d|n} d * μ(d) *
 *            ((-1)^{ω(n)} + μ(d)) * (σ(d)+φ(d)) For square‑free n, this
 *            simplifies further to iterating over all divisor subsets of...
 * Range:     u8 n <= 9, u16 n <= 208, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper: small prime sieve up to 10000 (enough for n <= 10000). */
static void _init_small_primes(void);
static uint64_t _small_primes[1229];
static size_t _small_prime_cnt = 0;

static void _init_small_primes(void)
{
    if (_small_prime_cnt != 0) return;
    const size_t limit = 10000;
    bool marked[limit + 1];
    for (size_t i = 0; i <= limit; ++i) marked[i] = false;
    for (size_t i = 2; i <= limit; ++i) {
        if (!marked[i]) {
            _small_primes[_small_prime_cnt++] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i) marked[j] = true;
            }
        }
    }
}

/* Factorise n into distinct primes (duplicates removed). */
static void _factor_distinct(uint64_t n, uint64_t *pr, size_t *cnt)
{
    _init_small_primes();
    size_t c = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < _small_prime_cnt && _small_primes[i] * _small_primes[i] <= tmp; ++i) {
        uint64_t p = _small_primes[i];
        if (tmp % p == 0) {
            pr[c++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) pr[c++] = tmp;
    *cnt = c;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 0) return 0;          /* offset is 1, but guard */
    uint64_t primes[128];          /* enough for any realistic n */
    size_t k;
    _factor_distinct((uint64_t)n, primes, &k);

    seq_t sum = 0;
    if (k == 0) {                  /* n == 1 */
        sum = (seq_t)4;            /* d=1 gives term 1*2*2 = 4 */
        return sum / 4;            /* =1 */
    }

    size_t total = (size_t)1 << k;
    for (size_t mask = 0; mask < total; ++mask) {
        size_t m = 0;
        seq_t d = 1, sigma = 1, phi = 1;
        for (size_t i = 0; i < k; ++i) {
            if (mask & ((size_t)1 << i)) {
                ++m;
                uint64_t p = primes[i];
                d *= (seq_t)p;
                sigma *= (seq_t)(1 + p);
                phi   *= (seq_t)(p - 1);
            }
        }
        /* Include only subsets with same parity as k. */
        if ((m & 1) != (k & 1)) continue;
        sum += d * (seq_t)2 * (sigma + phi);
    }
    return sum / 4;
}
