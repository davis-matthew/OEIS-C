/*
 * A324580  a(n) = n * A276086(n).
 * https://oeis.org/A324580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A276086(n), where A276086(n) = Product_{k>=0}
 *            prime(k+1)^d_k and d_k are the digits of n in primorial base;
 *            a(0) = 0
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 172, u64 n <= 1677,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9511 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const unsigned A324580_pr[17] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59};

seq_t a(idx_t n)
{
    seq_t r = 1;
    uint64_t m = (uint64_t)n;
    int k = 0;
    while (m > 0 && k < 17) {
        unsigned p = A324580_pr[k];
        uint64_t d = m % p;
        m /= p;
        for (uint64_t i = 0; i < d; i++)
            r *= (seq_t)A324580_pr[k];
        k++;
    }
    return r * (seq_t)n;
}
