/*
 * A248033  Least number k such that k + n divides F(n)^2 + F(k)^2.
 * https://oeis.org/A248033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A248033:
 *   a(n) = least k ≥ 1 such that (k + n) divides F(n)^2 + F(k)^2,
 *   where F(m) is the m‑th Fibonacci number.
 *   The implementation searches k increasing from 1,
 *   computing Fibonacci numbers modulo (k+n) by fast doubling.
 *
 *   The function works for any build of seq_t and idx_t.
 *   It relies only on simple arithmetic and the helper function
 *   to multiply 64‑bit integers modulo an unsigned 64‑bit integer.
 */
#include <stdint.h>
#include <stddef.h>

/* Return (a*b) mod mod, using 128‑bit intermediate to avoid overflow. */
static inline unsigned long long modmul64(unsigned long long a,
                                          unsigned long long b,
                                          unsigned long long mod) {
    return (unsigned __int128)a * b % mod;
}

/* Compute F(n) mod mod, with mod ≥ 2.  n fits in idx_t. */
static seq_t fib_mod_idx(idx_t n, idx_t mod) {
    unsigned long long m = (unsigned long long)mod;
    unsigned long long a = 0;   /* F(0) */
    unsigned long long b = 1;   /* F(1) */
    /* Determine number of bits of n */
    int bits = 0;
    idx_t tmp = n;
    while (tmp) {
        bits++;
        tmp >>= 1;
    }
    for (int i = bits - 1; i >= 0; --i) {
        /* c = F(2*i)  = a * (2*b - a) */
        unsigned long long two_b = (b << 1) % m;
        unsigned long long two_b_minus_a = (two_b + m - a) % m;
        unsigned long long c = modmul64(a, two_b_minus_a, m);
        /* d = F(2*i+1) = a*a + b*b */
        unsigned long long d = (modmul64(a, a, m) + modmul64(b, b, m)) % m;
        if ((n >> i) & 1) {
            a = d;
            b = (c + d) % m;
        } else {
            a = c;
            b = d;
        }
    }
    return (seq_t)a;
}

/* External function required by the build. */
seq_t a(idx_t n) {
    idx_t n_val = n;
    for (idx_t k = 1; ; ++k) {
        idx_t m = n_val + k;          /* m = k + n, with m ≥ 2 */
        seq_t fn = fib_mod_idx(n_val, m);
        seq_t fk = fib_mod_idx(k, m);

        /* Compute (fn^2 + fk^2) mod m safely */
        unsigned long long fn_ll = (unsigned long long)fn;
        unsigned long long fk_ll = (unsigned long long)fk;
        unsigned long long m_ll = (unsigned long long)m;
        unsigned long long term1 = modmul64(fn_ll, fn_ll, m_ll);
        unsigned long long term2 = modmul64(fk_ll, fk_ll, m_ll);
        unsigned long long sum = (term1 + term2) % m_ll;

        if (sum == 0)
            return (seq_t)k;
    }
}
