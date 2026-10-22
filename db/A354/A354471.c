/*
 * A354471  Number of fusion rings of rank 3 and multiplicity n.
 * https://oeis.org/A354471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = KroneckerDelta(n,1) + (1/2)*#{(k,l,p,q)∈{0,…,n}^4 : k²+l²
 *            = 1+l·p+k·q and max(1,k,l,p,q)=n}
 * Range:     u8 n <= 119, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Euclidean gcd for signed 64‑bit integers */
static inline int64_t llgcd(int64_t a, int64_t b)
{
    while (b) {
        int64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Extended Euclidean algorithm – returns g = gcd(a,b); x,y satisfy ax+by=g */
static inline int64_t extended_gcd(int64_t a, int64_t b, int64_t *x, int64_t *y)
{
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int64_t x1, y1;
    int64_t g = extended_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

/* modular inverse of a (mod m) – assumes gcd(a,m)==1, m>1 */
static inline int64_t modinv(int64_t a, int64_t m)
{
    int64_t x, y;
    extended_gcd(a, m, &x, &y);
    x %= m;
    if (x < 0)
        x += m;
    return x;
}

/* signed floor division (b>0) */
static inline int64_t floor_div(int64_t a, int64_t b)
{
    if (a >= 0) return a / b;
    return -(( -a + b - 1) / b);
}

/* signed ceil division (b>0) */
static inline int64_t ceil_div(int64_t a, int64_t b)
{
    if (a >= 0) return (a + b - 1) / b;
    return -((-a) / b);
}

/* Count all solutions (k,l,p,q) in [0,m]^4 satisfying k²+l² = 1+l·p+k·q, no restriction on max */
static uint64_t count_all_solutions_raw(idx_t m)
{
    uint64_t total = 0;

    if (m >= 1) {
        /* k = 0, l = 1  ->  p = 0, q ∈ [0,m]  :  m+1 solutions */
        total += (uint64_t)(m + 1);
        /* l = 0, k = 1  ->  q = 0, p ∈ [0,m]  :  m+1 solutions */
        total += (uint64_t)(m + 1);
    }

    if (m == 0)
        return total;   /* nothing else possible */

    for (idx_t k = 1; k <= m; ++k) {
        for (idx_t l = 1; l <= m; ++l) {
            if (llgcd((int64_t)k, (int64_t)l) != 1)
                continue;

            int64_t D = (int64_t)k * k + (int64_t)l * l - 1;

            int64_t p0, q0;
            if (k == 1) {
                p0 = 0;
                q0 = D;                /* k == 1 ⇒ divisions greet 1 */
            } else {
                int64_t inv = modinv((int64_t)l % (int64_t)k, (int64_t)k);
                p0 = ((D % (int64_t)k) * inv) % (int64_t)k;
                if (p0 < 0) p0 += k;
                q0 = (D - (int64_t)l * p0) / (int64_t)k;
            }

            /* determine t such that p=p0+t·k, q=q0−t·l stays within [0,m] */
            int64_t tmin1 = ceil_div(-p0, (int64_t)k);
            int64_t tmax1 = floor_div((int64_t)m - p0, (int64_t)k);
            int64_t tmin2 = ceil_div(q0 - (int64_t)m, (int64_t)l);
            int64_t tmax2 = floor_div(q0, (int64_t)l);

            int64_t tmin = tmin1 > tmin2 ? tmin1 : tmin2;
            int64_t tmax = tmax1 < tmax2 ? tmax1 : tmax2;

            if (tmin <= tmax)
                total += (uint64_t)(tmax - tmin + 1);
        }
    }

    return total;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    uint64_t Sn   = count_all_solutions_raw(n);
    uint64_t Snm1 = (n > 1) ? count_all_solutions_raw(n - 1) : 0;
    uint64_t diff = Sn - Snm1;          /* number of solutions with max = n */

    seq_t result = (seq_t)(diff / 2);
    if (n == 1) result += (seq_t)1;      /* KroneckerDelta(1,1) = 1 */

    return result;
}
