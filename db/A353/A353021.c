/*
 * A353021  a(n) = Sum_{l=1..n} Sum_{k=1..l} Sum_{j=1..k} Sum_{i=1..j}
 *          (l*k*j*i)^2.
 * https://oeis.org/A353021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(n+1)*(n+2)*(n+3)*(n+4)*(2*n+1)*(2*n+3)*(2*n+5)*(2*n+7)*(5*n-2)*(35*n^2-28*n+9)/5443200
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* small gcd routine for seq_t values */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* sequence function */
seq_t a(idx_t n)
{
    /* use seq_t values for all intermediate computations */
    seq_t fn = (seq_t)n;

    seq_t f1  = fn;                                         /* n               */
    seq_t f2  = fn + (seq_t)1;                               /* n+1             */
    seq_t f3  = fn + (seq_t)2;                               /* n+2             */
    seq_t f4  = fn + (seq_t)3;                               /* n+3             */
    seq_t f5  = fn + (seq_t)4;                               /* n+4             */
    seq_t f6  = (seq_t)2 * fn + (seq_t)1;                    /* 2*n+1           */
    seq_t f7  = (seq_t)2 * fn + (seq_t)3;                    /* 2*n+3           */
    seq_t f8  = (seq_t)2 * fn + (seq_t)5;                    /* 2*n+5           */
    seq_t f9  = (seq_t)2 * fn + (seq_t)7;                    /* 2*n+7           */
    seq_t f10 = (seq_t)5 * fn - (seq_t)2;                    /* 5*n-2           */
    seq_t f11 = (seq_t)35 * fn * fn - (seq_t)28 * fn + (seq_t)9; /* 35*n^2-28*n+9  */

    /* numerator factors */
    seq_t factors[11] = {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11};

    /* denominator: 5443200 = 2^7 * 3^5 * 5^2 * 7 */
    seq_t div = (seq_t)5443200;
    seq_t res = (seq_t)1;

    /* multiply each factor, cancelling with remaining denominator */
    for (int i = 0; i < 11; ++i) {
        seq_t v = factors[i];
        if (v == (seq_t)0)   /* any factor zero makes the whole product zero */
            return (seq_t)0;

        seq_t g = seq_gcd(v, div);
        if (g != (seq_t)1) {
            v   /= g;
            div  /= g;
        }
        res *= v;
    }

    /* any leftover divisor should divide evenly */
    if (div != (seq_t)1)
        res /= div;

    return res;
}
