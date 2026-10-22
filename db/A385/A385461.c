/*
 * A385461  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (8121*s + 28411) mod 134456 when
 *          started at 1.
 * https://oeis.org/A385461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8121*a(n-1) + 28411) mod 134456, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequences based on a 2×2 matrix exponentiation, running in O(log n). */

static const seq_t _A = 8121;
static const seq_t _B = 28411;
static const seq_t _M = 134456;

/* 2×2 matrix [ [a, b], [0, 1] ] used for linear congruential generator. */
typedef struct { seq_t a; seq_t b; } _mat2_t;

/* Matrix multiplication modulo _M :  [ [a1,b1], [0,1] ] * [ [a2,b2], [0,1] ] */
static _mat2_t _mat2_mul(const _mat2_t *m1, const _mat2_t *m2)
{
    _mat2_t r;
    r.a = (m1->a * m2->a) % _M;
    r.b = (m1->a * m2->b + m1->b) % _M;
    return r;
}

/* Fast exponentiation of the matrix to the power exp. */
static _mat2_t _mat2_pow(idx_t exp)
{
    _mat2_t res = { (seq_t)1, (seq_t)0 };  /* Identity matrix */
    _mat2_t base = { _A, _B };
    while (exp > 0) {
        if (exp & 1)
            res = _mat2_mul(&res, &base);
        base = _mat2_mul(&base, &base);
        exp >>= 1;
    }
    return res;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined by definition, return 0 */
    if (n == 1) return 1;            /* a(1) = 1 */

    _mat2_t m = _mat2_pow(n - 1);
    seq_t result = (m.a * (seq_t)1 + m.b) % _M;  /* a(1)=1 */
    return result;
}
