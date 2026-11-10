/*
 * A324651  Bisection of A324650: a(n) = A000010(A276086(2*n)).
 * https://oeis.org/A324651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000010(A276086(2*n)); computed as the product over
 *            primorial-base digits d_k of 2n of (p_k-1)*p_k^(d_k-1) for
 *            d_k>0, where p_k is the (k+1)-th prime
 * Range:     u8 n <= 10, u16 n <= 43, u32 n <= 298, u64 n <= 2307,
 *            u128 n <= 15015, big any n
 * Validated: standard, pass, 77 terms through n = 9939 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool a324651_is_prime(uint64_t p)
{
    uint64_t d;
    if (p < 2) return false;
    for (d = 2; d * d <= p; d++)
        if (p % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    unsigned __int128 m = (unsigned __int128)n * 2;
    seq_t result = 1;
    uint64_t p = 2;
    while (m > 0) {
        uint64_t d = (uint64_t)(m % p);
        m /= p;
        if (d > 0) {
            uint64_t i;
            result *= (seq_t)(p - 1);
            for (i = 1; i < d; i++)
                result *= (seq_t)p;
        }
        p++;
        while (!a324651_is_prime(p)) p++;
    }
    return result;
}
