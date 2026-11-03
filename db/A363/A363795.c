/*
 * A363795  Number of divisors of n of the form 7*k + 2.
 * https://oeis.org/A363795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9746 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of divisors of n that are congruent to 2 modulo 7.
   a(n) = #{ d : d | n , d ≡ 2 (mod 7) } */
#include <stdlib.h>

/* Helper: recursively enumerate all divisors from the factorisation
   primes[0..cnt-1] with exponents exps[0..cnt-1] and count those
   congruent to 2 mod 7. */
static void _enum_divs(const idx_t primes[], const int exps[],
                       int cnt, int idx, idx_t cur, seq_t *res)
{
    if (idx == cnt) {
        if ((cur % (idx_t)7) == (idx_t)2)
            (*res)++;
        return;
    }
    idx_t val = cur;
    /* iterate exponent 0 .. exps[idx] */
    for (int e = 0; e <= exps[idx]; ++e) {
        _enum_divs(primes, exps, cnt, idx + 1, val, res);
        /* multiply by the prime for the next exponent; careful to
           avoid overflow – val is always a divisor of n, thus <= n. */
        if (e < exps[idx])   /* avoid unnecessary multiplication at the end */
            val *= primes[idx];
    }
}

/* External linkage function required by the build */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)   /* undefined for n < 1 – return 0 */
        return (seq_t)0;

    /* 1. Factorise n with small trial division -> store primes and exponents */
    idx_t tmp = n;
    idx_t primes[64];
    int exps[64];
    int fac_cnt = 0;

    /* trial division by 2 */
    if ((tmp & (idx_t)1) == 0) {
        int e = 0;
        while ((tmp & (idx_t)1) == 0) {
            tmp >>= 1;
            ++e;
        }
        primes[fac_cnt] = (idx_t)2;
        exps[fac_cnt] = e;
        ++fac_cnt;
    }

    /* trial division by odd numbers */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == (idx_t)0) {
            int e = 0;
            while (tmp % p == (idx_t)0) {
                tmp /= p;
                ++e;
            }
            primes[fac_cnt] = p;
            exps[fac_cnt] = e;
            ++fac_cnt;
        }
    }

    if (tmp > 1) {          /* remaining prime factor */
        primes[fac_cnt] = tmp;
        exps[fac_cnt] = 1;
        ++fac_cnt;
    }

    /* 2. Enumerate divisors and count those ≡ 2 (mod 7) */
    seq_t result = (seq_t)0;
    _enum_divs(primes, exps, fac_cnt, 0, (idx_t)1, &result);

    return result;
}
