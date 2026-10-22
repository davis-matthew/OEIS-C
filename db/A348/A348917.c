/*
 * A348917  a(n) is the "w" part of f(n) = Sum_{k >= 0} g(d_k) * (4 + w)^k
 *          where g(0) = 0 and g(1 + u + 2*v) = (2 + w)^u * (1 + w)^v for any
 *          u = 0..1 and v = 0..5, Sum_{k >= 0} d_k * 13^k is the base-13
 *          representation of n and w = -1/2 + sqrt(-3)/2 is a primitive cube
 *          root of unity; sequence A348916 gives "r...
 * https://oeis.org/A348917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = w part of Σ_{k≥0} g(d_k)·(4+w)^k where d_k are the
 *            base‑13 digits of n
 * Range:     i8 n <= 2196, i16 n <= 2196, i32 n <= 2196, i64 n <= 2196,
 *            i128 n <= 2196, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

typedef struct {
    seq_t real;
    seq_t w;
} term_t;

static inline term_t mult(term_t x, term_t y)
{
    term_t r;
    r.real = x.real * y.real - x.w * y.w;
    r.w    = x.real * y.w + x.w * y.real - x.w * y.w;
    return r;
}

/* values of (1+w)^v for v = 0..5 */
static const term_t pow1[6] = {
    {  1,  0 }, /* v=0 */
    {  1,  1 }, /* v=1 */
    {  0,  1 }, /* v=2 */
    { -1,  0 }, /* v=3 */
    { -1, -1 }, /* v=4 */
    {  0, -1 }  /* v=5 */
};

/* g(d) for d = 0..12  (see problem statement) */
static const term_t g[13] = {
    {  0,  0 }, /* d=0  -> 0 */
    {  1,  0 }, /* d=1  -> (1+w)^0      */
    {  2,  1 }, /* d=2  -> (2+w)*(1+w)^0*/
    {  1,  1 }, /* d=3  -> (1+w)^1      */
    {  1,  2 }, /* d=4  -> (2+w)*(1+w)^1*/
    {  0,  1 }, /* d=5  -> (1+w)^2      */
    { -1,  1 }, /* d=6  -> (2+w)*(1+w)^2*/
    { -1,  0 }, /* d=7  -> (1+w)^3      */
    { -2, -1 }, /* d=8  -> (2+w)*(1+w)^3*/
    { -1, -1 }, /* d=9  -> (1+w)^4      */
    { -1, -2 }, /* d=10 -> (2+w)*(1+w)^4*/
    {  0, -1 }, /* d=11 -> (1+w)^5      */
    {  1, -1 }  /* d=12 -> (2+w)*(1+w)^5*/
};

seq_t a(idx_t n)
{
    term_t sum = { (seq_t)0, (seq_t)0 };      /* Σ g(d_k)*(4+w)^k  */
    term_t pow4 = { (seq_t)1, (seq_t)0 };    /* (4+w)^0 = 1       */
    term_t baseC = { (seq_t)4, (seq_t)1 };   /* (4+w)             */

    idx_t tmp = n;
    while (tmp > 0) {
        int d = (int)(tmp % 13);        /* current base‑13 digit */
        tmp /= 13;

        term_t prod = mult(g[d], pow4);
        sum.real += prod.real;
        sum.w    += prod.w;

        pow4 = mult(pow4, baseC);       /* advance to next power of (4+w) */
    }

    return sum.w;                       /* w part of f(n) */
}
