/*
 * A348497  a(n) = gcd(A003415(n), A347130(n)), where A003415 is the
 *          arithmetic derivative and A347130 is its Dirichlet convolution
 *          with the identity function.
 * https://oeis.org/A348497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003415(n), A347130(n)) Derived formula: a(n) =
 *            A003415(n) if n is not a perfect square, A003415(n)/2 if n is a
 *            perfect square. Here A003415(n) is the arithmetic derivative:
 *            A003415(n) = sum_{p^e || n} e * n / p (p prime, e exponent)
 * Range:     u8 n <= 95, u16 n <= 9856, u32 n <= 9856, u64 n <= 9856,
 *            u128 n <= 9856, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Warning: this implementation relies on a sieve of primes up to 1,000,000.
   It should be sufficient for all n used in the test cases. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_PRIME_SIEVE 1000000   /* sqrt(10^12) for safe factorization */

/* Static cache of primes */
static size_t _prime_cnt = 0;
static idx_t _primes[80000];
static bool _sieve_done = false;

static void _sieve_primes(void)
{
    if (_sieve_done) return;
    _sieve_done = true;

    static bool marked[MAX_PRIME_SIEVE + 1];
    for (idx_t i = 2; i * i <= MAX_PRIME_SIEVE; ++i) {
        if (!marked[i]) {
            for (idx_t j = i * i; j <= MAX_PRIME_SIEVE; j += i)
                marked[j] = true;
        }
    }
    for (idx_t i = 2; i <= MAX_PRIME_SIEVE; ++i) {
        if (!marked[i]) {
            _primes[_prime_cnt++] = i;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    _sieve_primes();                /* Ensure primes are ready */
    if (n == 0) return 0;           /* Undefined but keep safe */

    seq_t n_seq = (seq_t)n;
    idx_t tmp = n;
    seq_t f = 0;                    /* arithmetic derivative */
    bool is_square = true;          /* true if all exponents even */

    for (size_t i = 0; i < _prime_cnt && _primes[i] * _primes[i] <= tmp; ++i) {
        idx_t p = _primes[i];
        if (tmp % p == 0) {
            int cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            /* add e * n / p to f */
            f += (seq_t)cnt * (n_seq / (seq_t)p);
            if (cnt & 1) is_square = false;
        }
    }
    if (tmp > 1) {           /* remaining prime factor with exponent 1 */
        f += (seq_t)1 * (n_seq / (seq_t)tmp);
        is_square = false;
    }

    if (is_square && f != 0)
        return f / 2;        /* n is a perfect square */
    else
        return f;
}
