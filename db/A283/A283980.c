/*
 * A283980  a(n) = A003961(n)*A006519(n).
 * https://oeis.org/A283980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A003961(n)*A006519(n)
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 8191, u64 n <= 9305,
 *            u128 n <= 9305, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool is_prime_small(seq_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    seq_t d = 3;
    while (d <= x / d) {
        if (x % d == 0) return false;
        d += 2;
    }
    return true;
}

static seq_t next_prime(seq_t p) {
    seq_t np = p + 1;
    while (!is_prime_small(np)) ++np;
    return np;
}

static seq_t pow_seq_local(seq_t base, idx_t exp) {
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not used, but harmless */
    if (n == 1) return 1;                 /* empty product */

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* factorization by trial division */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p != 0) continue;
        idx_t exp = 0;
        while (m % p == 0) {
            ++exp;
            m /= p;
        }

        seq_t contrib;
        if (p == 2) {
            /* for 2^e : 6^e */
            contrib = pow_seq_local(6, exp);
        } else {
            /* for p^e : (next_prime(p))^e * 1 */
            contrib = pow_seq_local(next_prime((seq_t)p), exp);
        }
        result *= contrib;
    }

    /* remaining prime factor > sqrt(original) */
    if (m > 1) {
        seq_t p = (seq_t)m;
        idx_t exp = 1;
        seq_t contrib;
        if (p == 2) {
            contrib = pow_seq_local(6, exp);
        } else {
            contrib = pow_seq_local(next_prime(p), exp);
        }
        result *= contrib;
    }

    return result;
}
