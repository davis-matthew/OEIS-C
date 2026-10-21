/*
 * A270699  Integers n such that A084158(n) is divisible by n.
 * https://oeis.org/A270699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 3377, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A270699: Integers n such that A084158(n) is divisible by n.
   Using the identity A084158(n) = P_n * P_{n+1} / 2 where
   (P_1,P_2) = (1,2) and P_k = 2*P_{k-1} + P_{k-2}.
   Hence n is a term iff 2*n divides P_n * P_{n+1}.
   We test successive candidates and keep all found values in a static array.
   Matrix exponentiation is used to compute P_n modulo 2*n efficiently.
*/

#include <stdlib.h>
#include <stdint.h>

static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

typedef struct { uint64_t a[2][2]; } mat2_t;
static const mat2_t BASE_M = { { {2,1}, {1,0} } };

static mat2_t mat_mul(const mat2_t *x, const mat2_t *y, uint64_t mod) {
    mat2_t r;
    r.a[0][0] = (mul_mod(x->a[0][0], y->a[0][0], mod) +
                 mul_mod(x->a[0][1], y->a[1][0], mod)) % mod;
    r.a[0][1] = (mul_mod(x->a[0][0], y->a[0][1], mod) +
                 mul_mod(x->a[0][1], y->a[1][1], mod)) % mod;
    r.a[1][0] = (mul_mod(x->a[1][0], y->a[0][0], mod) +
                 mul_mod(x->a[1][1], y->a[1][0], mod)) % mod;
    r.a[1][1] = (mul_mod(x->a[1][0], y->a[0][1], mod) +
                 mul_mod(x->a[1][1], y->a[1][1], mod)) % mod;
    return r;
}

static mat2_t mat_pow(uint64_t exp, uint64_t mod) {
    mat2_t res = { { {1,0}, {0,1} } }; /* identity */
    mat2_t base = BASE_M;
    while (exp) {
        if (exp & 1UL) res = mat_mul(&res, &base, mod);
        base = mat_mul(&base, &base, mod);
        exp >>= 1UL;
    }
    return res;
}

/* Static storage for the sequence values and state */
static seq_t *seq_vals = NULL;
static size_t seq_vals_cap = 0;
static size_t seq_vals_len = 0;
static uint64_t seq_cur = 1;           /* next integer to test */

/* Ensure that seq_vals contains at least 'need' terms of the sequence */
static void ensure_size(idx_t need) {
    while (seq_vals_len < need) {
        uint64_t n = seq_cur++;
        uint64_t mod2 = n << 1;   /* 2 * n */

        /* Compute P_n and P_{n+1} modulo 2*n using matrix exponentiation */
        mat2_t Mpow = mat_pow(n - 1, mod2);
        uint64_t P_n    = Mpow.a[0][0] % mod2;
        uint64_t P_nm1  = Mpow.a[1][0] % mod2;
        uint64_t P_npl1 = (mul_mod(2, P_n, mod2) + P_nm1) % mod2;  /* P_{n+1} */

        uint64_t prod = mul_mod(P_n, P_npl1, mod2);
        if (prod == 0) {                         /* 2*n | P_n*P_{n+1}  */
            if (seq_vals_len == seq_vals_cap) {
                size_t new_cap = seq_vals_cap ? seq_vals_cap * 2 : 32;
                seq_t *new_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
                seq_vals = new_vals;
                seq_vals_cap = new_cap;
            }
            seq_vals[seq_vals_len++] = (seq_t)n;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined; return 0 for safety */
    ensure_size(n);
    return seq_vals[n - 1];
}
