/*
 * A384297  Consecutive internal states of the linear congruential
 *          pseudo-random number generator for Microsoft QBASIC when started
 *          at 1.
 * https://oeis.org/A384297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16598013 * a(n-1) + 12820163) mod 2^24, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required external types are supplied by the build system. */
#include <stdint.h>

/* Modulus 2^24 represented as a bit mask. */
static const uint64_t MOD_MASK = 0xFFFFFFULL;          /* 2^24 - 1 */

/* LCG constants (in 64‑bit unsigned for safety of intermediate products). */
static const uint64_t ALPHA = 16598013ULL;
static const uint64_t C     = 12820163ULL;

/* 2 × 2 matrix over the ring modulo 2^24. */
typedef struct {
    uint64_t a, b, c, d;   /* [ a b ; c d ] */
} mat_t;

/* Matrix multiplication modulo 2^24. */
static mat_t mat_mul(const mat_t *x, const mat_t *y)
{
    mat_t r;
    r.a = (x->a * y->a + x->b * y->c) & MOD_MASK;
    r.b = (x->a * y->b + x->b * y->d) & MOD_MASK;
    r.c = (x->c * y->a + x->d * y->c) & MOD_MASK;
    r.d = (x->c * y->b + x->d * y->d) & MOD_MASK;
    return r;
}

/* Fast exponentiation of a matrix: returns x^e. */
static mat_t mat_pow(mat_t base, idx_t exp)
{
    mat_t result = {1, 0, 0, 1};   /* Identity matrix. */
    while (exp) {
        if (exp & 1)
            result = mat_mul(&result, &base);
        base = mat_mul(&base, &base);
        exp >>= 1;
    }
    return result;
}

/* Sequence generator function. */
seq_t a(idx_t n)
{
    if (n == 0)               /* Undefined for n < 1, but return 0. */
        return (seq_t)0;

    /* Compute M^(n-1), where M = [ ALPHA  C ; 0 1 ]. */
    mat_t M = {ALPHA, C, 0, 1};
    mat_t P = mat_pow(M, n - 1);

    /* a(n) = P00 * a(1) + P01 * 1  (mod 2^24).   a(1) = 1. */
    uint64_t value = (P.a * 1ULL + P.b) & MOD_MASK;
    return (seq_t)value;
}
