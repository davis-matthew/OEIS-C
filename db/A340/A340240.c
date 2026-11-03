/*
 * A340240  Odd composite integers m such that A004254(3*m-J(m,21)) ==
 *          5*J(m,21) (mod m) and gcd(m,21)=1, where J(m,21) is the Jacobi
 *          symbol.
 * https://oeis.org/A340240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th odd composite integer m coprime with 21 such
 *            that A004254(3*m-J(m,21)) ≡ 5*J(m,21) (mod m)
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-03)
 * Helpers:   is_prime, mem, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented by sequential search with fast Lucas‑sequence evaluation.                          */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mulmod.h"
#include "../utils/mem.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

typedef struct { uint64_t a, b, c, d; } Mat;

static inline void mat_mul(Mat *out, const Mat *x, const Mat *y, uint64_t mod) {
    uint64_t a = (mulmod(x->a, y->a, mod) + mulmod(x->b, y->c, mod)) % mod;
    uint64_t b = (mulmod(x->a, y->b, mod) + mulmod(x->b, y->d, mod)) % mod;
    uint64_t c = (mulmod(x->c, y->a, mod) + mulmod(x->d, y->c, mod)) % mod;
    uint64_t d = (mulmod(x->c, y->b, mod) + mulmod(x->d, y->d, mod)) % mod;
    out->a = a; out->b = b; out->c = c; out->d = d;
}

/* Compute Lucas sequence U(n) (parameters a=5, b=1) modulo mod.  */
static uint64_t lucas_mod(uint64_t n, uint64_t mod) {
    Mat res = {1, 0, 0, 1};                     /* Identity matrix */
    Mat base = {5 % mod, (mod - 1) % mod, 1 % mod, 0};
    while (n) {
        if (n & 1ULL) {
            Mat tmp; mat_mul(&tmp, &res, &base, mod); res = tmp;
        }
        n >>= 1;
        if (n) {
            Mat tmp; mat_mul(&tmp, &base, &base, mod); base = tmp;
        }
    }
    return res.c;  /* U(n) is at position [1][0] of M^n */
}

seq_t a(idx_t n) {
    static seq_t *terms = NULL;
    static size_t cap = 0;
    static idx_t count = 0;
    static uint64_t cand = 1;      /* last odd number tested */

    while (count < n) {
        uint64_t m;
        /* Find next odd composite coprime with 21 */
        do {
            cand += 2;                    /* keep cand odd */
            if (cand % 3 == 0 || cand % 7 == 0) continue;
            if (is_prime(cand)) continue;
            m = cand;
            break;
        } while (1);

        int J = jacobi_symbol(m, 21);     /* J = ±1 because gcd(m,21)=1 */
        if (J == 0) continue;            /* safety, should not happen */

        uint64_t N = (J == 1) ? 3ull * m - 1 : 3ull * m + 1;
        uint64_t val = lucas_mod(N, m);

        uint64_t rhs = (J == 1) ? (5ull % m) : (m - (5ull % m));

        if (val == rhs) {
            if (count >= cap) {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, terms, newcap);
                terms = tmp;
                cap = newcap;
            }
            terms[count++] = (seq_t)m;
        }
    }
    return terms[n - 1];
}
