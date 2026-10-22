/*
 * A329145  Number of non-necklace compositions of n.
 * https://oeis.org/A329145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) - (A000031(n) - 1) where A000031(n) = (1/n) *
 *            sum_{d|n} phi(d) * 2^{n/d}
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"

static seq_t A000031(idx_t n)
{
    /* factorise n */
    idx_t prime[10], exp[10];
    size_t pc = 0;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            prime[pc] = p;
            exp[pc]  = e;
            ++pc;
        }
    }
    if (m > 1) {
        prime[pc] = m;
        exp[pc]  = 1;
        ++pc;
    }

    /* generate all divisors of n */
    idx_t divs[256];
    size_t dcnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < pc; ++i) {
        idx_t p  = prime[i];
        int    e = exp[i];
        size_t old = dcnt;
        for (size_t j = 0; j < old; ++j) {
            idx_t base = divs[j];
            idx_t mult = 1;
            for (int f = 1; f <= e; ++f) {
                mult *= p;
                divs[dcnt++] = base * mult;
            }
        }
    }

    /* sum terms */
    seq_t sum = 0;
    for (size_t k = 0; k < dcnt; ++k) {
        idx_t d = divs[k];

        /* compute phi(d) */
        idx_t phi = d;
        idx_t r   = d;
        for (idx_t p = 2; p * p <= r; ++p) {
            if (r % p == 0) {
                while (r % p == 0) r /= p;
                phi -= phi / p;
            }
        }
        if (r > 1) phi -= phi / r;

        seq_t term = pow2(n / d);
        term *= (seq_t)phi;
        sum += term;
    }

    seq_t result = sum / (seq_t)n;   /* exact division */
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but handle safely */

    seq_t two_pow = pow2(n - 1);           /* 2^(n-1) */
    seq_t necklaces = A000031(n);          /* A000031(n) */
    seq_t res = two_pow - necklaces + 1;  /* a(n) = 2^(n-1) - A000031(n) + 1 */
    return res;
}
