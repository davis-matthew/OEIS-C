/*
 * A353642  Even bisection of A353640.
 * https://oeis.org/A353642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A342002(2*n) mod 4, where A342002(m) = Sum_{i} d_i *
 *            Product_{j != i, d_j > 0} p_j, with d_i the digits of m in
 *            primorial (mixed radix prime) base
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9863 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static const unsigned A353642_primes[] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
    31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u,
    73u, 79u, 83u, 89u, 97u, 101u, 103u, 107u, 109u, 113u
};

seq_t a(idx_t n)
{
    idx_t m = n * 2;
    if (m == 0) return 0;
    unsigned digits[32];
    size_t cnt = 0;
    while (m > 0 && cnt < 30) {
        unsigned p = A353642_primes[cnt];
        digits[cnt] = (unsigned)(m % p);
        m /= p;
        ++cnt;
    }
    seq_t result = 0;
    for (size_t j = 0; j < cnt; ++j) {
        if (digits[j] == 0) continue;
        seq_t term = (seq_t)digits[j];
        for (size_t i = 0; i < cnt; ++i) {
            if (i != j && digits[i] != 0) term *= (seq_t)A353642_primes[i];
        }
        result += term;
    }
    return result % 4;
}
