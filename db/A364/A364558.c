/*
 * A364558  a(n) = A364557(n) - A000010(n), where A364557 is the Möbius
 *          transform of A005941, and A000010 (Euler phi) is the Möbius
 *          transform of A000027.
 * https://oeis.org/A364558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A364557(n) - A000010(n), where A364557(n) = 2^((# of 2's
 *            in n)-1 + Sum_{odd primes p^e || n} (e-1) + (pi(max odd prime
 *            factor)-1)) for n > 1, A364557(1) = 1, and A000010 is Euler phi
 * Range:     i8 n <= 22, i16 n <= 58, i32 n <= 136, i64 n <= 312,
 *            i128 n <= 726, big any n
 * Validated: standard, pass, 89 terms through n = 9961 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t A364558_pi(idx_t x)
{
    if (x < 2)
        return 0;
    unsigned char *comp = (unsigned char *)calloc((size_t)x + 1, 1);
    if (!comp)
        return 0;
    idx_t cnt = 0;
    for (idx_t i = 2; i <= x; i++) {
        if (!comp[i]) {
            cnt++;
            if (i <= x / i)
                for (idx_t j = i * i; j <= x; j += i)
                    comp[j] = 1;
        }
    }
    free(comp);
    return cnt;
}

seq_t a(idx_t n)
{
    idx_t m = n;
    seq_t phi = 1;
    idx_t exponent = 0;
    idx_t max_odd = 0;
    bool has_two = false;

    for (idx_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            idx_t e = 0;
            seq_t pk = 1;
            while (m % p == 0) {
                m /= p;
                e++;
                if (e > 1)
                    pk *= (seq_t)p;
            }
            phi *= (seq_t)(p - 1) * pk;
            if (p == 2) {
                has_two = true;
                exponent += e - 1;
            } else {
                exponent += e - 1;
                max_odd = p;
            }
        }
    }
    if (m > 1) {
        phi *= (seq_t)(m - 1);
        if (m == 2)
            has_two = true;
        else
            max_odd = m;
    }
    (void)has_two;

    if (max_odd > 0)
        exponent += A364558_pi(max_odd) - 1;

    seq_t val = 1;
    for (idx_t i = 0; i < exponent; i++)
        val *= 2;
    return val - phi;
}
