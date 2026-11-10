/*
 * A366801  Arithmetic derivative without its inherited divisor applied to the
 *          Doudna sequence: a(n) = A342001(A005940(1+n)).
 * https://oeis.org/A366801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A342001(A005940(1+n)); with A005940(1+n) = prod
 *            prime(k)^e_k, e_k = # of 1's in binary n followed by k-1 zeros;
 *            a(n) = Sum_k e_k * rad/prime(k), rad = product of the primes
 *            with e_k > 0
 * Range:     u8 n <= 83, u16 n <= 2643, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 102 terms through n = 9829 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    unsigned pr[80];
    unsigned e[80];
    int np = 0;
    for (unsigned c = 2; np < 80; ++c) {
        int ok = 1;
        for (int j = 0; j < np && pr[j] * pr[j] <= c; ++j)
            if (c % pr[j] == 0) { ok = 0; break; }
        if (ok) pr[np++] = c;
    }
    for (int j = 0; j < 80; ++j) e[j] = 0;

    idx_t m = n;
    int i = 0;
    while (m > 0) {
        if (m & 1) {
            e[i]++;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }

    seq_t rad = (seq_t)1;
    for (int j = 0; j < 80; ++j)
        if (e[j]) rad *= (seq_t)pr[j];

    seq_t result = (seq_t)0;
    for (int j = 0; j < 80; ++j)
        if (e[j]) result += (seq_t)e[j] * (rad / (seq_t)pr[j]);
    return result;
}
