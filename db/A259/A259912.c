/*
 * A259912  Discriminant of the field of the number having constant continued
 *          fraction [n,n,n,...].
 * https://oeis.org/A259912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = discriminant of the quadratic field of the number with
 *            constant continued fraction [n,n,n,…]
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- Prime sieve up to √(max D) ---------- */
#define MAX_PRIME_LIMIT 10010   /* > √(10000²+4) */

static uint64_t _primes[1200] = {0};   /* enough primes up to 10010 */
static size_t   _prime_cnt  = 0;
static bool     _primes_ready = false;

static void _init_primes(void)
{
    if (_primes_ready) return;
    bool sieve[MAX_PRIME_LIMIT];
    for (size_t i = 0; i < MAX_PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i < MAX_PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j < MAX_PRIME_LIMIT; j += i) sieve[j] = false;
        }
    }
    for (size_t i = 2; i < MAX_PRIME_LIMIT; ++i) {
        if (sieve[i]) _primes[_prime_cnt++] = i;
    }
    _primes_ready = true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Ensure primes are ready (only once) */
    _init_primes();

    /* Compute D = n² + 4      */
    seq_t N = (seq_t)n;
    seq_t D = N * N + (seq_t)4;

    /* Find the square‑free part d of D   */
    seq_t temp = D;
    seq_t d = (seq_t)1;

    for (size_t idx = 0; idx < _prime_cnt; ++idx) {
        seq_t p = _primes[idx];
        if (p * p > temp) break;
        uint64_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }
        if (exp & 1) d *= p;          /* keep an odd number of p’s */
    }
    if (temp > 1) d *= temp;           /* remaining prime factor */

    /* Field discriminant:  Δ = d          (if d ≡ 1 (mod4))
                          or Δ = 4·d      (otherwise)    */
    return ((d & (seq_t)3) == (seq_t)1) ? d : (d << 2);
}
