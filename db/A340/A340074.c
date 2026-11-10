/*
 * A340074  a(n) = gcd(A003961(n)-1, A339904(n)).
 * https://oeis.org/A340074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003961(n)-1, A339904(n)); A003961 completely
 *            multiplicative with prime(k)->prime(k+1); A339904 multiplicative
 *            with p^e -> oddpart(q-1)*q^(e-1), q=nextprime(p)
 * Range:     u8 n <= 55, u16 n <= 7343, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 121 terms through n = 7571 (2026-11-10)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

static seq_t A340074_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    seq_t shifted = 1;   /* A003961(n) */
    seq_t odd = 1;       /* A339904(n) */
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            uint64_t o = q - 1;
            while ((o & 1) == 0) o >>= 1;
            odd *= (seq_t)o;
            int first = 1;
            while (m % p == 0) {
                m /= p;
                shifted *= (seq_t)q;
                if (!first) odd *= (seq_t)q;
                first = 0;
            }
        }
    }
    if (m > 1) {
        uint64_t q = next_prime(m);
        uint64_t o = q - 1;
        while ((o & 1) == 0) o >>= 1;
        odd *= (seq_t)o;
        shifted *= (seq_t)q;
    }
    return A340074_gcd(shifted - 1, odd);
}
