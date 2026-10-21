/*
 * A251990  Numbers n such that the sum of the hexagonal numbers H(n) and
 *          H(n+1) is equal to the sum of the pentagonal numbers P(m) and
 *          P(m+1) for some m.
 * https://oeis.org/A251990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A + 2*B - 1) / 4, where (A,B) is the pair *
 *            representation of r^n with r = 97 + 56*sqrt(3): * (A +
 *            B*sqrt(3)) = r^n. * The recurrence a(n) = 195*a(n-1) -
 *            195*a(n-2) + a(n-3) follows from * this closed form
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

struct rpair { seq_t a; seq_t b; };

static inline struct rpair pair_mul(struct rpair x, struct rpair y)
{
    struct rpair res;
    /* res.a = x.a * y.a + 3 * x.b * y.b */
    res.a = x.a * y.a + 3 * x.b * y.b;
    /* res.b = x.a * y.b + x.b * y.a */
    res.b = x.a * y.b + x.b * y.a;
    return res;
}

static inline struct rpair pair_pow(idx_t exp)
{
    struct rpair result = { (seq_t)1, (seq_t)0 };      /* r^0 */
    struct rpair base   = { (seq_t)97, (seq_t)56 };     /* r^1 */
    while (exp) {
        if (exp & 1ULL)
            result = pair_mul(result, base);
        base = pair_mul(base, base);
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute A,B such that r^n = A + B*sqrt(3) */
    struct rpair r = pair_pow(n);
    /* a(n) = (A + 2*B - 1) / 4 */
    seq_t twoB   = r.b + r.b;     /* 2*B */
    seq_t sum    = r.a + twoB - (seq_t)1;
    return sum / (seq_t)4;
}
