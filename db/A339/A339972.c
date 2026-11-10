/*
 * A339972  Odd part of phi(A019565(8*n)).
 * https://oeis.org/A339972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   If 16n = 2^e1 + 2^e2 + ... + 2^ek [e1 ... ek distinct], then
 *            a(n) = A057023(e1) * A057023(e2) * ... * A057023(ek), where
 *            A057023(e) is the odd part of prime(e)-1
 * Range:     u8 n <= 22, u16 n <= 182, u32 n <= 4086, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 90 terms through n = 9881 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint32_t A339972_prime_nth(unsigned k)
{
    /* k-th prime, k >= 1, by trial division */
    unsigned count = 0;
    uint32_t c = 1;
    while (count < k) {
        ++c;
        bool ok = true;
        for (uint32_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) ++count;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    unsigned bit = 0;
    while (n != 0) {
        if (n & 1ULL) {
            uint32_t v = A339972_prime_nth(bit + 4) - 1;
            while ((v & 1u) == 0) v >>= 1;
            result *= (seq_t)v;
        }
        n >>= 1;
        ++bit;
    }
    return result;
}
