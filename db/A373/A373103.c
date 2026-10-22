/*
 * A373103  a(n) = Sum_{1 <= x_1, x_2, x_3, x_4, x_5 <= n} ( n/gcd(x_1, x_2,
 *          x_3, x_4, x_5, n) )^4.
 * https://oeis.org/A373103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{1 <= x1,...,x5 <= n} ( n/gcd(x1,..,x5,n) )^4
 *            Multiplicative implementation using prime factorisation: a(p^e)
 *            = (p^(9e+9) - p^(9e+4) + p^4 - 1)/(p^9-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute b^e for small e with seq_t arithmetic */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/* a(p^e) for a single prime power */
static seq_t a_prime_pow(uint32_t p, idx_t e)
{
    seq_t p4  = pow_seq((seq_t)p, 4);
    seq_t p4m1 = p4 - (seq_t)1;
    seq_t p9  = pow_seq((seq_t)p, 9);

    seq_t term = (seq_t)1;  /* p^(9*(k-1)) */
    seq_t sum  = (seq_t)1;  /* sum_{k=0}^{e-1} p^(9k) */

    for (idx_t i = 1; i < e; ++i) {
        term *= p9;      /* term = p^(9i) */
        sum  += term;    /* add to sum */
    }

    seq_t pow9e = term * p9;          /* term = p^(9*(e-1)) then *p9 = p^(9e) */
    seq_t res   = pow9e - (p4m1 * sum);
    return res;
}

/* Smallest prime factor array, lazily built up to the largest n seen. */
static idx_t _spf_n = 0;
static uint32_t *_spf = NULL;

static void _ensure_spf(idx_t limit)
{
    if (_spf && limit <= _spf_n)
        return;

    free(_spf);
    _spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    if (!_spf) return;  /* allocation failure – caller will get 0 */

    for (idx_t i = 0; i <= limit; ++i)
        _spf[i] = (uint32_t)i;
    if (limit >= 1) _spf[1] = 1;

    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (_spf[i] == i) {          /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i)
                if (_spf[j] == (uint32_t)j)
                    _spf[j] = (uint32_t)i;
        }
    }
    _spf_n = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined, but never called with 0 */

    _ensure_spf(n);

    idx_t tmp = n;
    seq_t result = (seq_t)1;

    while (tmp > 1) {
        uint32_t p = _spf[tmp];
        idx_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        result *= a_prime_pow(p, e);
    }

    return result;
}
