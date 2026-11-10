/*
 * A244319  Self-inverse permutation of natural numbers: a(1) = 1, a(2n) =
 *          A003961(1+a(A064989(2n-1))), a(2n+1) =
 *          1+A003961(a(A064989(2n+1)-1)).
 * https://oeis.org/A244319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2n) = A003961(1+a(A064989(2n-1))), a(2n+1) =
 *            1+A003961(a(A064989(2n+1)-1))
 * Range:     u8 n <= 16, u16 n <= 132, u32 n <= 1893, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 78 terms through n = 9870 (2026-11-07)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

static uint64_t A244319_prev_prime(uint64_t p)
{
    uint64_t q = p - 1;
    while (q > 2 && !is_prime_u64(q)) --q;
    return q;
}

/* A064989 on a native index */
static uint64_t A244319_A064989(uint64_t n)
{
    uint64_t r = 1;
    while (n % 2 == 0) n /= 2;
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t q = A244319_prev_prime(p);
            while (n % p == 0) {
                n /= p;
                r *= q;
            }
        }
    }
    if (n > 1) r *= A244319_prev_prime(n);
    return r;
}

/* A003961 on a seq_t value */
static seq_t A244319_A003961(seq_t m)
{
    seq_t result = 1;
    uint64_t p = 2;
    while ((seq_t)p * (seq_t)p <= m) {
        if (m % p == 0) {
            seq_t q = (seq_t)next_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= q;
            }
        }
        p += (p == 2) ? 1 : 2;
    }
    if (m > 1) {
        result *= (seq_t)next_prime((uint64_t)m);
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    if (n % 2 == 0) {
        uint64_t m = A244319_A064989((uint64_t)n - 1);
        seq_t v = a((idx_t)m);
        return A244319_A003961(v + 1);
    } else {
        uint64_t m = A244319_A064989((uint64_t)n) - 1;
        seq_t v = a((idx_t)m);
        return A244319_A003961(v) + 1;
    }
}
