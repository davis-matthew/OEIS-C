/*
 * A260210  A034602(n) modulo prime(n).
 * https://oeis.org/A260210
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((C(2p-1, p-1) - 1)/p^3) mod p where C denotes the
 *            binomial coefficient and p = prime(n) (nth prime)
 * Range:     u8 n <= 55, u16 n <= 6639, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Helper: modular multiplication using 128‑bit arithmetic to avoid overflow */
static inline __int128 mul_mod(__int128 a, __int128 b, __int128 mod) {
    __int128 res = 0;
    a %= mod;
    b %= mod;
    while (b > 0) {
        if (b & 1)
            res = (res + a) % mod;
        a = (a << 1) % mod;  /* a = a*2 % mod */
        b >>= 1;
    }
    return res;
}

/* Helper: extended Euclidean algorithm for 128‑bit integers */
static inline __int128 egcd(__int128 a, __int128 b, __int128 *x, __int128 *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    __int128 x1, y1;
    __int128 g = egcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

/* Helper: modular inverse of a modulo mod (gcd(a,mod)=1) */
static inline __int128 mod_inv(__int128 a, __int128 mod) {
    __int128 x, y;
    egcd(a, mod, &x, &y);
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 3) return 0;                 /* offset 3 rules; undefined below */

    uint32_t p = get_prime(n);           /* nth prime */
    __int128 p128 = (__int128)p;

    /* Compute modulus p^4 */
    __int128 mod = p128 * p128 * p128 * p128;

    __int128 res = 1;

    /* Compute C(2p-1, p-1) mod p^4 via multiplicative formula */
    for (idx_t k = 1; k < p; ++k) {
        __int128 num = p128 + k;                 /* numerator term (p + k) */
        res = mul_mod(res, num, mod);
        __int128 inv = mod_inv((__int128)k, mod);
        res = mul_mod(res, inv, mod);
    }

    /* (C - 1) / p^3  modulo p */
    __int128 t = (res - 1) / (p128 * p128 * p128);   /* exact division */
    __int128 a_val = t % p128;                       /* reduce modulo p */

    return (seq_t)a_val;
}
