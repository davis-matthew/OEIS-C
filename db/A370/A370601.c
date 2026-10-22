/*
 * A370601  a(n) is the number of integer values of the function F_2n(x) =
 *          Product_{i=0..2n-1} (x + i) / Sum_{i=0..2n-1} (x + i).
 * https://oeis.org/A370601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau((D²)/gcd(D²,n)) – n, where D = (2n‑1)!!
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   _vp_odd_double_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/_vp_odd_double_factorial.h"

/* ---- prime sieve up to 20000 (covers n ≤ 10000) ---- */
#define MAX_SIEVE 20000
static uint8_t _is_comp[MAX_SIEVE + 1];
static uint32_t _primes[3000];
static size_t _prime_cnt = 0;

static void _init_primes(void)
{
    if (_prime_cnt > 0) return;                /* already done */
    for (uint32_t i = 2; i <= MAX_SIEVE; ++i) {
        if (!_is_comp[i]) {
            _primes[_prime_cnt++] = i;
            if ((uint64_t)i * i <= MAX_SIEVE) {
                for (uint32_t j = i * i; j <= MAX_SIEVE; j += i)
                    _is_comp[j] = 1;
            }
        }
    }
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside offset, safe return */

    _init_primes();

    seq_t tau = 1;
    idx_t two_n_minus1 = 2 * n - 1;

    for (size_t pi = 0; pi < _prime_cnt; ++pi) {
        uint32_t p = _primes[pi];
        if (p == 2) continue;           /* 2 never divides (2n‑1)!! */
        if ((idx_t)p > two_n_minus1) break;

        /* exponent of p in D = (2n‑1)!! */
        size_t vp_D = _vp_odd_double_factorial(n, p);

        /* exponent of p in n */
        size_t vp_n = 0;
        idx_t temp = n;
        while (temp % p == 0) {
            vp_n++;
            temp /= p;
        }

        /* exponent of p in D² */
        size_t e_p = 2 * vp_D;

        /* exponent of p in gcd(D² , n) is min(e_p , vp_n) */
        size_t exp = e_p - (vp_n < e_p ? vp_n : e_p);

        /* factor (exp + 1) into tau */
        tau = tau * (seq_t)(exp + 1);
    }

    return tau - (seq_t)n;
}
