/*
 * A363806  Number of divisors of n of the form 7*k + 4.
 * https://oeis.org/A363806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 757, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _max_divisors_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A363806: a(n) = number of divisors of n of the form 7*k + 4 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_max_divisors_sqrt.h"

/* static prime table up to 10 000, enough for all n ≤ 10 000 */
static uint64_t _primes[1229];
static size_t   _prime_cnt = 0;
static bool     _primes_initialized = false;

static void _init_primes(void)
{
    if (_primes_initialized) return;
    const size_t LIMIT = 10000;
    bool is_comp[LIMIT + 1];
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            _primes[_prime_cnt++] = i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    _primes_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for out‑of‑bounds calls */

    _init_primes();

    /* factorise n */
    uint64_t m = (uint64_t)n;
    uint64_t base[10];
    uint8_t  exp[10];
    size_t   pf_cnt = 0;

    for (size_t i = 0; i < _prime_cnt; ++i) {
        uint64_t p = _primes[i];
        if (p * p > m) break;
        if (m % p != 0) continue;
        uint8_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        base[pf_cnt] = p;
        exp [pf_cnt] = e;
        ++pf_cnt;
    }
    if (m > 1) {          /* remaining prime factor */
        base[pf_cnt] = m;
        exp [pf_cnt] = 1;
        ++pf_cnt;
    }

    /* enumerate all divisors */
    /* the maximum number of divisors for n ≤ 10 000 is < 200 */
    seq_t divs[200];
    size_t div_cnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = base[i];
        uint8_t   e = exp[i];
        /* precompute powers of p */
        seq_t powers[10];     /* e ≤ 10 for n ≤ 10 000 */
        powers[0] = 1;
        for (uint8_t k = 1; k <= e; ++k) powers[k] = powers[k-1] * (seq_t)p;

        /* for each existing divisor, multiply by all powers of p */
        size_t old_cnt = div_cnt;
        for (size_t d = 0; d < old_cnt; ++d) {
            seq_t val = divs[d];
            for (uint8_t k = 1; k <= e; ++k) {
                if (div_cnt >= 200) continue;     /* safety guard */
                divs[div_cnt++] = val * powers[k];
            }
        }
    }

    /* count divisors congruent to 4 mod 7 */
    seq_t cnt = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        if ((idx_t)divs[i] % 7 == 4) ++cnt;
    }
    return cnt;
}
