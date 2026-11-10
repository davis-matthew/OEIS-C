/*
 * A295891  a(n) = 1 if binary weights of n and A003961(n) are of the
 *          different parity, 0 otherwise; a(n) = A010060(n) XOR
 *          A010060(A003961(n)).
 * https://oeis.org/A295891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A000120(n) + A000120(A003961(n))) mod 2, where A003961
 *            is completely multiplicative with a(prime(k)) = prime(k+1)
 * Range:     u8 n <= 47, u16 n <= 8231, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 136 terms through n = 9714 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static seq_t A003961_a(idx_t n) {
    seq_t result = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= (seq_t)q;
            }
        }
    }
    if (m > 1) {
        result *= (seq_t)next_prime(m);
    }
    return result;
}

seq_t a(idx_t n) {
    int par = __builtin_popcountll((unsigned long long)n) & 1;
    seq_t x = A003961_a(n);
    while (x > 0) {
        par ^= (int)(long long)(x & 1);
        x >>= 1;
    }
    return (seq_t)par;
}
