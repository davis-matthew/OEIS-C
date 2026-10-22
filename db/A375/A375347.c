/*
 * A375347  a(n) is the number of nonnegative numbers k < n such that the
 *          congruence x^k == k (mod n) is solvable.
 * https://oeis.org/A375347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k < n such that the equation x^k ≡ k (mod n)
 *            has a solution. Equivalently, k is a kth power residue modulo n
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Euclidean gcd for unsigned 64-bit values. */
static inline uint64_t _gcd_u64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined case; return 0 */
    seq_t count = 0;

    /* k = 0 is solvable only when n = 1 */
    if (n == 1) {
        return 1;                 /* only k = 0 satisfies */
    }

    /* Factor n into prime powers */
    uint64_t nn = (uint64_t)n;
    uint64_t prime[16];
    unsigned expv[16];
    size_t pcnt = 0;
    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            unsigned e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            prime[pcnt] = p;
            expv[pcnt] = e;
            ++pcnt;
        }
    }
    if (nn > 1) {
        prime[pcnt] = nn;
        expv[pcnt] = 1;
        ++pcnt;
    }

    /* Iterate over all k from 1 to n-1 */
    for (idx_t k = 1; k < n; ++k) {
        bool ok = true;
        uint64_t kraw = (uint64_t)k;

        for (size_t i = 0; i < pcnt && ok; ++i) {
            uint64_t p = prime[i];
            unsigned e = expv[i];

            /* modulus p^e */
            uint64_t mod = 1;
            for (unsigned t = 0; t < e; ++t) mod *= p;
            uint64_t phi = mod / p * (p - 1);   /* φ(p^e) */

            if ((kraw % p) == 0) {
                /* brute‑force check for this prime power */
                bool found = false;
                for (uint64_t x = 0; x < mod; ++x) {
                    seq_t r = mod_pow((seq_t)x, kraw, (seq_t)mod);
                    if (r == (seq_t)(kraw % mod)) {
                        found = true;
                        break;
                    }
                }
                if (!found) ok = false;
            } else {
                /* k coprime to p: use the kth‑power residue test */
                uint64_t g = _gcd_u64(kraw, phi);
                uint64_t exp = phi / g;
                seq_t v = mod_pow((seq_t)(kraw % mod), exp, (seq_t)mod);
                if (v != 1) ok = false;
            }
        }
        if (ok) ++count;
    }

    return count;
}
