/*
 * A385359  Consecutive states of the linear congruential pseudo-random number
 *          generator (1597*s + 51749) mod 244944 when started at s=1.
 * https://oeis.org/A385359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (1597*a(n-1) + 51749) mod 244944
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t MOD = (seq_t)244944ULL;
static const seq_t A   = (seq_t)1597ULL;
static const seq_t B   = (seq_t)51749ULL;

/* modular addition */
static seq_t mod_add(seq_t x, seq_t y) {
    unsigned long long sum = (unsigned long long)(x % MOD) + (unsigned long long)(y % MOD);
    return (seq_t)(sum % MOD);
}

/* modular multiplication */
static seq_t mod_mul(seq_t x, seq_t y) {
    unsigned long long prod = (unsigned long long)(x % MOD) * (unsigned long long)(y % MOD);
    return (seq_t)(prod % MOD);
}

/* multiply two 2x2 matrices over the modulus */
static void mat_mul(seq_t r[2][2], const seq_t a[2][2], const seq_t b[2][2]) {
    seq_t t00 = mod_add(mod_mul(a[0][0], b[0][0]), mod_mul(a[0][1], b[1][0]));
    seq_t t01 = mod_add(mod_mul(a[0][0], b[0][1]), mod_mul(a[0][1], b[1][1]));
    seq_t t10 = mod_add(mod_mul(a[1][0], b[0][0]), mod_mul(a[1][1], b[1][0]));
    seq_t t11 = mod_add(mod_mul(a[1][0], b[0][1]), mod_mul(a[1][1], b[1][1]));
    r[0][0] = t00;  r[0][1] = t01;
    r[1][0] = t10;  r[1][1] = t11;
}

/* exponentiate the transition matrix to the power (pow) */
static void mat_pow(seq_t res[2][2], idx_t pow) {
    seq_t identity[2][2] = {{(seq_t)1, (seq_t)0}, {(seq_t)0, (seq_t)1}};
    seq_t base[2][2]     = {{A, B}, {(seq_t)0, (seq_t)1}};
    /* copy identity to res */
    res[0][0] = identity[0][0]; res[0][1] = identity[0][1];
    res[1][0] = identity[1][0]; res[1][1] = identity[1][1];
    seq_t temp[2][2];

    while (pow) {
        if (pow & (idx_t)1) {
            mat_mul(temp, res, base);
            res[0][0] = temp[0][0]; res[0][1] = temp[0][1];
            res[1][0] = temp[1][0]; res[1][1] = temp[1][1];
        }
        mat_mul(temp, base, base);
        base[0][0] = temp[0][0]; base[0][1] = temp[0][1];
        base[1][0] = temp[1][0]; base[1][1] = temp[1][1];
        pow >>= (idx_t)1;
    }
}

seq_t a(idx_t n) {
    if (n == (idx_t)1) {
        return (seq_t)1;
    }
    seq_t mat[2][2];
    mat_pow(mat, n - (idx_t)1);          /* mat = transition^(n-1) */
    /* a(n) = mat[0][0] * a(1) + mat[0][1] * 1 (mod MOD) */
    seq_t term1 = mod_mul(mat[0][0], (seq_t)1);
    seq_t term2 = mat[0][1];            /* already modded */
    return mod_add(term1, term2);
}
