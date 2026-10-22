/*
 * A309364  a(n) is the least k >= 0 such that n divides C(k) (where C(k) are
 *          the Catalan numbers A000108).
 * https://oeis.org/A309364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 0 such that n | C(k), where C(k) is the k‑th
 *            Catalan number
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _exp_catalan, _exp_factorial, _exp_in_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_exp_catalan.h"
#include "../utils/_exp_factorial.h"
#include "../utils/_exp_in_int.h"

/* ---------- prime utilities ---------- */

#define PRIME_SIEVE_LIMIT 10000          /* enough for all n ≤ 10 000 */
static uint32_t _primes[1229];           /* 1229 primes ≤ 10000 */
static size_t _prime_cnt = 0;
static bool _primes_built = false;

static void _build_primes(void)
{
    if (_primes_built) return;
    bool is_comp[PRIME_SIEVE_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_SIEVE_LIMIT; ++i) is_comp[i] = false;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            _primes[_prime_cnt++] = (uint32_t)i;
            if (i * i <= PRIME_SIEVE_LIMIT) {
                for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    _primes_built = true;
}

/* factorize n (≤ UINT64_MAX) into distinct primes and exponents.
   Arrays `pr` and `exp` must be big enough (at least 10). */
static size_t _factorize(idx_t n, uint64_t pr[10], uint64_t exp[10])
{
    _build_primes();
    size_t cnt = 0;
    idx_t tmp = n;
    for (size_t i = 0; i < _prime_cnt && (uint64_t)_primes[i] * _primes[i] <= tmp; ++i) {
        uint64_t p = _primes[i];
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pr[cnt] = p;
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {                      /* remaining prime factor > sqrt(n) */
        pr[cnt] = tmp;
        exp[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* ---------- public sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;     /* by definition */

    uint64_t primes[10], exps[10];
    size_t pcnt = _factorize(n, primes, exps);

    /* search k from 0 upwards */
    for (idx_t k = 0; ; ++k) {
        bool ok = true;
        for (size_t i = 0; i < pcnt; ++i) {
            if (_exp_catalan(k, primes[i]) < exps[i]) {
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)k;
    }
}
