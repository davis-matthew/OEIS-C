/*
 * A379489  a(n) = gcd(n,A003961(n))*gcd(sigma(n),A276086(n)) -
 *          gcd(n,A276086(n))*gcd(sigma(n),A003961(n)), where A003961 is fully
 *          multiplicative with a(prime(i)) = prime(i+1), and A276086 is the
 *          primorial base exp-function.
 * https://oeis.org/A379489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = gcd(n, A003961(n))*gcd(sigma(n),A276086(n)) - gcd(n,
 *            A276086(n))*gcd(sigma(n),A003961(n))
 * Range:     i8 n <= 97, i16 n <= 1619, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 103 terms through n = 9804 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A379489: a(n) = gcd(n, A003961(n)) gcd(sigma(n), A276086(n))
 *               - gcd(n, A276086(n)) gcd(sigma(n), A003961(n)).
 * A003961(n) = Prod nextprime(p)^e over n = Prod p^e, and A276086(n) =
 * Prod prime(i)^d_i over the primorial-base digits d_i of n: both can be huge,
 * but a gcd with a number x only needs their exponents,
 * gcd(x, Prod q^f) = Prod q^min(v_q(x), f), so nothing large is formed. (The
 * original's tables were sized for n <= 10000.) */

#include <stdint.h>

static uint64_t vq(uint64_t x, uint64_t q, uint64_t f)     /* q^min(v_q(x), f) */
{
    uint64_t r = 1;
    while (f-- && x % q == 0) { x /= q; r *= q; }
    return r;
}

static int is_prime(uint64_t m)
{
    if (m < 2) return 0;
    for (uint64_t d = 2; d * d <= m; ++d) if (m % d == 0) return 0;
    return 1;
}

static uint64_t next_prime(uint64_t p)
{
    do ++p; while (!is_prime(p));
    return p;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    const uint64_t N = (uint64_t)n;
    /* n = Prod p^e, sigma(n), and the factors of A003961(n) */
    uint64_t P[16], E[16], Q[16];
    int np = 0;
    uint64_t t = N, sigma = 1;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p) continue;
        uint64_t e = 0, pe = 1, s = 1;
        while (t % p == 0) { t /= p; ++e; pe *= p; s += pe; }
        P[np] = p; E[np] = e; Q[np] = next_prime(p); ++np;
        sigma *= s;
    }
    if (t > 1) { P[np] = t; E[np] = 1; Q[np] = next_prime(t); ++np; sigma *= t + 1; }
    /* gcd(x, A003961(n)) */
    uint64_t g_n_s = 1, g_s_s = 1;
    for (int i = 0; i < np; ++i) {
        g_n_s *= vq(N, Q[i], E[i]);
        g_s_s *= vq(sigma, Q[i], E[i]);
    }
    /* gcd(x, A276086(n)): the primorial-base digits of n */
    uint64_t g_n_p = 1, g_s_p = 1, m = N, q = 2;
    while (m) {
        uint64_t d = m % q;
        m /= q;
        if (d) {
            g_n_p *= vq(N, q, d);
            g_s_p *= vq(sigma, q, d);
        }
        q = next_prime(q);
    }
    return (seq_t)((int64_t)(g_n_s * g_s_p) - (int64_t)(g_n_p * g_s_s));
}
