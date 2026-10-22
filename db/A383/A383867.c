/*
 * A383867  The sum of divisors d of n having the property that for every
 *          prime p dividing n the p-adic valuation of d is either 0 or a
 *          squarefree divisor of the p-adic valuation of n.
 * https://oeis.org/A383867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} ( 1 + Σ_{d|e, d squarefree} p^d )
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square‑free test for small numbers (e ≤ 64) */
static inline bool is_squarefree_int(int v) {
    for (int p = 2; p * p <= v; ++p) {
        if (v % p == 0) {
            if ((v / p) % p == 0) return false;  /* p^2 | v */
            while (v % p == 0) v /= p;
        }
    }
    return true;  /* remaining prime factor (if any) is squarefree */
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;   /* a(1) = 1, undefined for 0 */

    idx_t temp = n;
    seq_t result = (seq_t)1;

    /* factor 2 separately */
    int exp = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        exp++;
    }
    if (exp > 0) {
        seq_t local = (seq_t)1;
        seq_t pow   = (seq_t)1;   /* p^0 */
        for (int d = 1; d <= exp; ++d) {
            pow *= (seq_t)2;       /* 2^d */
            if (exp % d == 0 && is_squarefree_int(d))
                local += pow;
        }
        result *= local;
    }

    /* trial division by odd numbers, up to sqrt(temp) */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            seq_t local = (seq_t)1;
            seq_t pow   = (seq_t)1;
            for (int d = 1; d <= exp; ++d) {
                pow *= (seq_t)p;
                if (exp % d == 0 && is_squarefree_int(d))
                    local += pow;
            }
            result *= local;
        }
    }

    /* if leftover is a prime > 1 */
    if (temp > 1) {
        seq_t local = (seq_t)1 + (seq_t)temp;  /* e = 1: squarefree divisors are {1} */
        result *= local;
    }

    return result;
}
