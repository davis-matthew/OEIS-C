/*
 * A387459  a(n) = Sum_{k=0..n} (n-i)^k * (n+i)^(n-k), where i is the
 *          imaginary unit.
 * https://oeis.org/A387459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Im((n + i)^(n+1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef struct {
    seq_t mag;
    int   sign;          /* +1 or -1, zero is stored with sign +1 */
} signed_seq_t;

static inline signed_seq_t signed_seq_make(seq_t mag, int sign) {
    signed_seq_t r;
    r.mag  = mag;
    r.sign = (mag == 0) ? 1 : sign;
    return r;
}

/* signed addition: a + b */
static inline signed_seq_t signed_seq_add(signed_seq_t a, signed_seq_t b) {
    if (a.sign == b.sign) {
        return signed_seq_make(a.mag + b.mag, a.sign);
    } else {
        if (a.mag > b.mag) {
            return signed_seq_make(a.mag - b.mag, a.sign);
        } else if (b.mag > a.mag) {
            return signed_seq_make(b.mag - a.mag, b.sign);
        } else {
            return signed_seq_make(0, 1);
        }
    }
}

/* signed subtraction: a - b */
static inline signed_seq_t signed_seq_sub(signed_seq_t a, signed_seq_t b) {
    b.sign = -b.sign;
    return signed_seq_add(a, b);
}

/* signed multiplication: a * b */
static inline signed_seq_t signed_seq_mul(signed_seq_t a, signed_seq_t b) {
    if (a.mag == 0 || b.mag == 0)
        return signed_seq_make(0, 1);
    int sign = a.sign * b.sign;
    return signed_seq_make(a.mag * b.mag, sign);
}

/* signed complex number: r + i*i */
typedef struct {
    signed_seq_t r;
    signed_seq_t i;
} signed_complex_t;

/* multiplication of signed complex numbers */
static inline void signed_complex_mul(const signed_complex_t *a,
                                      const signed_complex_t *b,
                                      signed_complex_t *out) {
    signed_seq_t pr = signed_seq_mul(a->r, b->r);
    signed_seq_t pi = signed_seq_mul(a->i, b->i);
    out->r = signed_seq_sub(pr, pi);

    signed_seq_t pr2 = signed_seq_mul(a->r, b->i);
    signed_seq_t pi2 = signed_seq_mul(a->i, b->r);
    out->i = signed_seq_add(pr2, pi2);
}

/* exponentiation by squaring: (base) ^ exp */
static inline signed_complex_t signed_complex_pow(const signed_complex_t *base,
                                                 idx_t exp) {
    signed_complex_t result;
    result.r = signed_seq_make(1, 1);
    result.i = signed_seq_make(0, 1);

    signed_complex_t cur = *base;
    while (exp) {
        if (exp & 1) {
            signed_complex_t tmp;
            signed_complex_mul(&result, &cur, &tmp);
            result = tmp;
        }
        exp >>= 1;
        if (exp) {
            signed_complex_t tmp;
            signed_complex_mul(&cur, &cur, &tmp);
            cur = tmp;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    /* (n + i)^(n+1) and take its imaginary part */
    signed_complex_t base;
    base.r = signed_seq_make(n, 1);
    base.i = signed_seq_make(1, 1);

    signed_complex_t res = signed_complex_pow(&base, n + 1);
    /* Imaginary part magnitude is the result (always positive) */
    return res.i.mag;
}
