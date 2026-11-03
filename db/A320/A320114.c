/*
 * A320114  a(n) = Product_{d|n, d>1} prime(1+(d mod 4)).
 * https://oeis.org/A320114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d>1} prime(1+(d mod 4))
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 209, u64 n <= 1259,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 90 terms through n = 7449 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: recursively enumerate all divisors of n built from the
   factor lists primes[0..cnt-1] and exp[0..cnt-1].
   For each divisor > 1, multiply the running product by the prime
   corresponding to its residue modulo 4: 2 for r=0, 3 for r=1,
   5 for r=2, 7 for r=3. */
static void _product_divisors(int idx, const uint64_t *pr,
                              const int *exp, int cnt,
                              uint64_t cur, seq_t *res)
{
    if (idx == cnt) {
        if (cur > 1) {
            int rem = cur % 4;
            seq_t mult;
            if (rem == 0)          mult = 2;
            else if (rem == 1)     mult = 3;
            else if (rem == 2)     mult = 5;
            else                   /* rem == 3 */ mult = 7;
            *res = (*res) * mult;
        }
        return;
    }

    uint64_t prod = 1;
    for (int k = 0; k <= exp[idx]; ++k) {
        _product_divisors(idx + 1, pr, exp, cnt, cur * prod, res);
        prod *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    uint64_t tn = (uint64_t)n;
    uint64_t primes[20];
    int exp[20];
    int cnt = 0;

    /* factor 2 */
    if (tn % 2 == 0) {
        int e = 0;
        while (tn % 2 == 0) {
            tn /= 2;
            ++e;
        }
        primes[cnt] = 2;
        exp[cnt] = e;
        ++cnt;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= tn / p; p += 2) {
        if (tn % p == 0) {
            int e = 0;
            while (tn % p == 0) {
                tn /= p;
                ++e;
            }
            primes[cnt] = p;
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (tn > 1) {
        primes[cnt] = tn;
        exp[cnt] = 1;
        ++cnt;
    }

    seq_t result = 1;
    _product_divisors(0, primes, exp, cnt, 1, &result);
    return result;
}
