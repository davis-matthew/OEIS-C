/*
 * A245327  Numerators in recursive bijection from positive integers to
 *          positive rationals, where the bijection is f(1) = 1, f(2n) =
 *          1/(f(n)+1), f(2n+1) = f(n)+1.
 * https://oeis.org/A245327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of f(n) where f(1) = 1, f(2n) = 1 / (f(n) + 1),
 *            f(2n+1) = f(n) + 1 (indices start at n = 1)
 * Range:     u8 n <= 4096, u16 n <= 9487, u32 n <= 9487, u64 n <= 9487,
 *            u128 n <= 9487, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

typedef struct {
    seq_t num;
    seq_t den;
} pair_t;

/* Recursive helper that returns the numerator and denominator of f(n). */
static pair_t _pair_rec(idx_t n)
{
    if (n == 1) {
        pair_t r;
        r.num = (seq_t)1;
        r.den = (seq_t)1;
        return r;
    }
    pair_t p = _pair_rec(n >> 1);
    if ((n & 1) == 0) {          /* even: f(2k) = b/(a+b) */
        pair_t r;
        r.num = p.den;
        r.den = p.num + p.den;
        return r;
    } else {                     /* odd:  f(2k+1) = (a+b)/b */
        pair_t r;
        r.num = p.num + p.den;
        r.den = p.den;
        return r;
    }
}

/* Public entry point: returns the numerator of f(n). */
seq_t a(idx_t n)
{
    /* Indices below the offset are undefined; return 0 for safety. */
    if (n == 0) {
        return (seq_t)0;
    }
    return _pair_rec(n).num;
}
