/*
 * A297845  Encoded multiplication table for polynomials in one indeterminate
 *          with nonnegative integer coefficients. Symmetric square array T(n,
 *          k) read by antidiagonals, n > 0 and k > 0. See comment for
 *          details.
 * https://oeis.org/A297845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, mult
 * Formula:   a(n) is the nth term of the array read by antidiagonals of the *
 *            symmetric multiplicative table T(n,k) defined by T(1,k)=1 and *
 *            T(p_i, k)=∏_{q^e||k} p_{i+index(q)-1}^e, where p_i denotes the
 *            i‑th prime
 * Range:     u8 n <= 97, u16 n <= 480, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   _coords_from_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/_coords_from_index.h"

#define MAX_PRIME_LIMIT 5000  /* enough to cover indices up to ~500 */

static bool _primes_initialized = false;
static int  _prime_array[MAX_PRIME_LIMIT];      /* primes in order */
static uint16_t _prime_index_map[MAX_PRIME_LIMIT + 1]; /* 0 if not prime, otherwise 1‑based index */
static int _prime_count = 0;

/* initialize prime tables */
static void _init_primes(void)
{
    if (_primes_initialized) return;

    bool is_prime[MAX_PRIME_LIMIT + 1];
    memset(is_prime, 1, sizeof(is_prime));
    is_prime[0] = is_prime[1] = 0;

    for (int i = 2; i * i <= MAX_PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= MAX_PRIME_LIMIT; j += i) {
                is_prime[j] = false;
            }
        }
    }

    _prime_count = 0;
    for (int i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            ++_prime_count;
            _prime_array[_prime_count - 1] = i;
            _prime_index_map[i] = _prime_count;  /* 1‑based index */
        }
    }
    _primes_initialized = true;
}

/* small factorisation: returns number of prime factors in out arrays */
static int _factor_small(idx_t n,
                        uint32_t primes_out[8],
                        uint32_t exps_out[8])
{
    int cnt = 0;
    idx_t m = n;

    for (int pi = 0; pi < _prime_count && (idx_t)_prime_array[pi] * (idx_t)_prime_array[pi] <= m; ++pi) {
        uint32_t p = (uint32_t)_prime_array[pi];
        if (m % p == 0) {
            uint32_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes_out[cnt] = p;
            exps_out[cnt] = e;
            ++cnt;
            if (cnt >= 8) break;  /* enough for our bounds */
        }
    }
    if (m > 1) {
        primes_out[cnt] = (uint32_t)m;
        exps_out[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* binary exponentiation of seq_t */
static seq_t _pow_seq(seq_t base, uint32_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    uint32_t e = exp;
    while (e) {
        if (e & 1U) result *= cur;
        e >>= 1;
        if (e) cur *= cur;
    }
    return result;
}

/* compute T(n,k) */
static seq_t _T(idx_t n, idx_t k)
{
    if (n == 1 || k == 1) return 1;

    uint32_t np[8], ne[8], nk[8], ne2[8];
    int cn = _factor_small(n, np, ne);
    int ck = _factor_small(k, nk, ne2);

    seq_t result = 1;

    for (int i = 0; i < cn; ++i) {
        uint32_t pi = np[i];
        uint32_t ei = ne[i];
        uint16_t idx_i = _prime_index_map[pi];  /* 1‑based index of prime pi */

        for (int j = 0; j < ck; ++j) {
            uint32_t qj = nk[j];
            uint32_t ej = ne2[j];
            uint16_t idx_j = _prime_index_map[qj];  /* 1‑based index of prime qj */

            uint32_t exponent = ei * ej;
            if (exponent == 0) continue;

            int shift_index = (int)idx_i + (int)idx_j - 1;   /* 1‑based */
            if (shift_index <= 0 || shift_index > _prime_count) {
                /* Should not happen for the intended input range. */
                return 0;
            }
            seq_t prime_val = (seq_t)_prime_array[shift_index - 1];
            result *= _pow_seq(prime_val, exponent);
        }
    }
    return result;
}

/* public sequence function */
seq_t a(idx_t n)
{
    _init_primes();                /* ensure primes available */
    idx_t i, j;
    _coords_from_index(n, &i, &j);
    return _T(i, j);
}
