/*
 * A296362  Number of monohedral disk tilings of type C^t_{5,n}.
 * https://oeis.org/A296362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Sum_{i=0..10} A241926(i, n*(10-i)) for n > 1, a(1) = 2,
 *            where A241926(p,q) = (1/(p+q)) * Sum_{d | gcd(p,q)}
 *            phi(d)*binomial((p+q)/d, p/d) (= 1 when p = 0 or q = 0). It is
 *            computed in the overflow-free form A241926(p,q) =
 *            Prod_{j=1..p-1}(N-p+j)/p! + (1/p)*Sum_{d|g, d>1}
 *            phi(d)*binomial(N/...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 44 terms through n = 865 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t a296362_gcd(idx_t x, idx_t y)
{
    while (y != 0) {
        idx_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static idx_t a296362_phi(idx_t m)
{
    idx_t r = m;
    idx_t d;
    for (d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            r -= r / d;
        }
    }
    if (m > 1) r -= r / m;
    return r;
}

static seq_t a296362_binom(idx_t n, idx_t k)
{
    idx_t i;
    seq_t res = 1;
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    for (i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* necklaces with p black and q white beads, without large intermediates */
static seq_t a296362_T(idx_t p, idx_t q)
{
    idx_t N, g, D, i, d, r, rem;
    seq_t qq = 0, Y = 0, c;
    if (p == 0 || q == 0) return 1;
    N = p + q;
    g = a296362_gcd(p, q);

    D = 1;
    for (i = 2; i <= p; ++i) D *= i;          /* p! */

    /* main term: Prod_{j=1..p-1}(N-p+j) / p!, kept as qq*D + r */
    r = 1;
    qq += (seq_t)(r / D);
    r %= D;
    for (i = 1; i + 1 <= p; ++i) {
        idx_t f = N - p + i;
        idx_t t;
        qq = qq * (seq_t)f;
        t = r * f;
        qq += (seq_t)(t / D);
        r = t % D;
    }

    /* small terms from divisors d > 1 of g */
    for (d = 2; d <= g; ++d) {
        if (g % d == 0)
            Y += (seq_t)a296362_phi(d) * a296362_binom(N / d - 1, p / d - 1);
    }
    /* Y/p = Y*(p-1)!/p! */
    c = Y * (seq_t)(D / p);
    qq += c / (seq_t)D;
    rem = (idx_t)(long long)(c % (seq_t)D);
    r += rem;
    qq += (seq_t)(r / D);
    return qq;
}

seq_t a(idx_t n)
{
    idx_t i;
    seq_t s = 0;
    if (n == 1) return 2;
    for (i = 0; i <= 10; ++i) {
        s += a296362_T(i, n * (10 - i));
    }
    return s * 2;
}
