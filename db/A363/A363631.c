/*
 * A363631  Expansion of Sum_{k>0} (1/(1+x^k)^4 - 1).
 * https://oeis.org/A363631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{d|n} (-1)^d * binomial(d+3,3)
 * Range:     i8 n <= 7, i16 n <= 55, i32 n <= 2231, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Computes a(n) for OEIS A363631 */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* unlikely, but safe */

    /* Factorisation: collect prime factors and exponents */
    idx_t pf[64];           /* prime factors */
    int   pe[64];           /* exponents */
    int   pfcnt = 0;

    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            pf[pfcnt] = p;
            pe[pfcnt] = e;
            ++pfcnt;
        }
    }
    if (temp > 1) {                    /* remaining prime factor */
        pf[pfcnt]   = temp;
        pe[pfcnt]   = 1;
        ++pfcnt;
    }

    /* Generate all divisors using the prime factorisation */
    idx_t divs[4096];    /* enough for n up to 10^12; adjust if needed */
    size_t size = 1;
    divs[0] = 1;

    for (int i = 0; i < pfcnt; ++i) {
        idx_t prime = pf[i];
        int   e     = pe[i];
        size_t curr_size = size;
        for (size_t j = 0; j < curr_size; ++j) {
            idx_t base = divs[j];
            idx_t mult = 1;
            for (int k = 1; k <= e; ++k) {
                mult *= prime;
                divs[size++] = base * mult;
            }
        }
    }

    /* Sum over divisors */
    seq_t sum = 0;
    for (size_t i = 0; i < size; ++i) {
        idx_t d = divs[i];
        seq_t bin = ((seq_t)(d + 3) * (d + 2) * (d + 1)) / 6;
        seq_t sign = (d & 1) ? -1 : 1;   /* (-1)^d */
        sum += sign * bin;
    }

    return sum;
}
