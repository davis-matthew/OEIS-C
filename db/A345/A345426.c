/*
 * A345426  For 1<=x<=n, 1<=y<=n, write gcd(x,y) = u*x+v*y with u,v minimal;
 *          a(n) = sum of the values of u.
 * https://oeis.org/A345426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∑_{x=1}^n ∑_{y=1}^n u(x,y) where u(x,y) is the
 *            coefficient of x in the Bézout identity gcd(x,y) = u*x + v*y
 *            with |u|^2+|v|^2 minimal (u=0,v=1 when x=y)
 * Range:     i8 n <= 49, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* minimal u for coprime a,b (a,b ≥ 1) */
static inline int minimal_u(int a, int b)
{
    /* handling the special case b==1 */
    if (b == 1) return 0;            /* then a==1 as well, giving u=0 */

    /* Extended Euclid: find x,y with a*x + b*y = 1 */
    int old_s = 1, s = 0;
    int old_t = 0, t = 1;
    int aa = a, bb = b;

    while (bb != 0) {
        int q = aa / bb;
        int tmp_s = old_s - q * s;
        old_s = s;
        s = tmp_s;

        int tmp_t = old_t - q * t;
        old_t = t;
        t = tmp_t;

        int tmp = aa - q * bb;
        aa = bb;
        bb = tmp;
    }
    /* aa == 1, old_s is one inverse of a modulo b */
    int u0 = old_s % b;
    if (u0 < 0) u0 += b;
    /* map to range (-b/2, b/2] */
    if (u0 > b / 2) return u0 - b;
    return u0;
}
/* helper: gcd for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t x = 1; x <= n; ++x) {
        for (idx_t y = 1; y <= n; ++y) {
            idx_t g = gcd_idx(x, y);
            idx_t a1 = x / g;
            idx_t b1 = y / g;
            int u = minimal_u((int)a1, (int)b1);
            sum += (seq_t)u;
        }
    }
    return sum;
}
