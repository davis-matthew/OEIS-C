/*
 * A352037  Sum of the 9th powers of the odd proper divisors of n.
 * https://oeis.org/A352037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d<n, d odd} d^9
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 25, u64 n <= 277,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;

    /* factorize the odd part of n */
    idx_t m = n;
    while ((m & 1) == 0) m >>= 1;   /* strip all factors of 2 */

    /* store prime factors and exponents */
    idx_t primes[32];
    idx_t exps[32];
    size_t pf_cnt = 0;

    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pf_cnt] = p;
            exps[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (m > 1) {
        primes[pf_cnt] = m;
        exps[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* generate all odd divisors of n (via odd part) */
    seq_t divs[256];
    size_t div_cnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        idx_t p = primes[i];
        idx_t e = exps[i];
        size_t current = div_cnt;
        for (size_t j = 0; j < current; ++j) {
            seq_t base = divs[j];
            seq_t mult = 1;
            for (idx_t k = 1; k <= e; ++k) {
                mult = mult * (seq_t)p;
                divs[div_cnt++] = base * mult;
                if (div_cnt >= sizeof(divs)/sizeof(divs[0])) break;  /* safety */
            }
            if (div_cnt >= sizeof(divs)/sizeof(divs[0])) break;
        }
    }

    /* sum d^9 for d<n */
    seq_t total = 0;
    seq_t n_seq = (seq_t)n;
    for (size_t i = 0; i < div_cnt; ++i) {
        seq_t d = divs[i];
        if (d < n_seq) {
            total += big_pow(d, (seq_t)9);
        }
    }

    return total;
}
