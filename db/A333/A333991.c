/*
 * A333991  a(n) = Sum_{k=0..n} (-n)^k * binomial(2*n,2*k).
 * https://oeis.org/A333991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-n)^k * binomial(2*n,2*k)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute a(n) via binary exponentiation in the ring Z[√‑n]. */
    /*  Let w² = –n.  Then (1 + w)²ⁿ has real part equal to the required sum. */
    typedef struct { seq_t a, b; } pair;

    seq_t N = (seq_t)n;          /* cast index to seq_t */
    /* initialise result = 1 (real part 1, imaginary part 0) */
    pair  result = { (seq_t)1, (seq_t)0 };
    /* base = 1 + w  (real part 1, imaginary part 1) */
    pair  base    = { (seq_t)1, (seq_t)1 };

    idx_t e = n << 1;            /* exponent 2*n */
    while (e) {
        if (e & 1) {
            /* result = result * base */
            seq_t a1 = result.a * base.a;
            seq_t a2 = result.b * base.b;
            seq_t a3 = N * a2;                /* N * (b1*b2) */
            seq_t new_a = a1 - a3;
            seq_t new_b = result.a * base.b
                        + result.b * base.a;
            result.a = new_a;
            result.b = new_b;
        }
        /* base = base * base */
        seq_t a1 = base.a * base.a;
        seq_t a2 = base.b * base.b;
        seq_t a3 = N * a2;
        seq_t new_a = a1 - a3;
        seq_t new_b = base.a * base.b
                    + base.b * base.a;
        base.a = new_a;
        base.b = new_b;
        e >>= 1;
    }
    return result.a; /* real part of (1 + √‑n)²ⁿ */
}
