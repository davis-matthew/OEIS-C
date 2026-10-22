/*
 * A348916  a(n) is the "real" part of f(n) = Sum_{k >= 0} g(d_k) * (4 + w)^k
 *          where g(0) = 0 and g(1 + u + 2*v) = (2 + w)^u * (1 + w)^v for any
 *          u = 0..1 and v = 0..5, Sum_{k >= 0} d_k * 13^k is the base-13
 *          representation of n and w = -1/2 + sqrt(-3)/2 is a primitive cube
 *          root of unity; sequence A348917 gives...
 * https://oeis.org/A348916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) is the real part of f(n) = Σ_{k≥0} g(d_k) * (4 + w)^k where
 *            w = -½ + (√-3)/2, g(0) = 0, g(1+u+2v) = (2 + w)^u * (1 + w)^v
 *            (u∈{0,1}, v∈{0..5}), and d_k are the base‑13 digits of n (least
 *            significant first)
 * Range:     i8 n <= 2196, i16 n <= 2196, i32 n <= 2196, i64 n <= 2196,
 *            i128 n <= 2196, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const struct { seq_t a; seq_t b; } g[13] = {
    {  0,  0},  // d = 0
    {  1,  0},  // d = 1
    {  2,  1},  // d = 2
    {  1,  1},  // d = 3
    {  1,  2},  // d = 4
    {  0,  1},  // d = 5
    { -1,  1},  // d = 6
    { -1,  0},  // d = 7
    { -2, -1},  // d = 8
    { -1, -1},  // d = 9
    { -1, -2},  // d =10
    {  0, -1},  // d =11
    {  1, -1}   // d =12
};

seq_t a(idx_t n)
{
    seq_t sum   = 0;        // cumulative real part
    seq_t pow_a = 1;        // real part of (4 + w)^k
    seq_t pow_b = 0;        // w part of (4 + w)^k

    idx_t m = n;
    while (m != 0) {
        unsigned d = (unsigned)(m % 13);
        m /= 13;

        seq_t ga = g[d].a;
        seq_t gb = g[d].b;

        sum += ga * pow_a - gb * pow_b;

        // update power: (4 + w) * (pow_a + pow_b w)
        seq_t new_a = 4 * pow_a - pow_b;
        seq_t new_b = pow_a + 3 * pow_b;
        pow_a = new_a;
        pow_b = new_b;
    }
    return sum;
}
