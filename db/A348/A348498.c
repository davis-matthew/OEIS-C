/*
 * A348498  a(n) = gcd(A003415(n), A347130(n)) / A003557(n), where A003415 is
 *          the arithmetic derivative and A347130 is its Dirichlet convolution
 *          with the identity function.
 * https://oeis.org/A348498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003415(n), A347130(n)) / A003557(n)
 * Range:     u8 n <= 317, u16 n <= 9851, u32 n <= 9851, u64 n <= 9851,
 *            u128 n <= 9851, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003415(n)  : arithmetic derivative of n               */
/* A347130(n)  : Dirichlet convolution of A003415 with the identity function */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Factorisation: stores primes and their exponents in the supplied arrays.
   The caller must provide arrays large enough (64 slots are plenty for n <= 10000). */
static void factor(idx_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    *cnt = 0;
    uint64_t m = (uint64_t)n;
    if (m % 2 == 0) {
        uint64_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        pr[*cnt] = 2ULL;
        ex[*cnt] = e;
        ++(*cnt);
    }
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            pr[*cnt] = p;
            ex[*cnt] = e;
            ++(*cnt);
        }
    }
    if (m > 1) {
        pr[*cnt] = m;
        ex[*cnt] = 1;
        ++(*cnt);
    }
}

/* Compute gcd for 128‑bit unsigned integers (Euclidean algorithm). */
static unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != 0) {
        unsigned __int128 r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Recursive enumeration of divisors of n.
   At leaf, compute contribution to g(n) and add to *g_acc. */
static void gen_divisors(const uint64_t *pr, const uint64_t *ex,
                         size_t num, size_t idx,
                         unsigned __int128 curr_d,
                         int *exp_curr,
                         unsigned __int128 n,
                         unsigned __int128 *g_acc)
{
    if (idx == num) {
        /* Compute f(d) = sum e_i * (d / p_i) for this divisor d = curr_d. */
        unsigned __int128 f_d = 0;
        for (size_t i = 0; i < num; ++i) {
            int e = exp_curr[i];
            if (e > 0) {
                f_d += (unsigned __int128)e * (curr_d / (unsigned __int128)pr[i]);
            }
        }
        /* Add to g: f(d) * (n / d) */
        *g_acc += f_d * (n / curr_d);
        return;
    }

    /* Iterate exponent for current prime factor. */
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        exp_curr[idx] = (int)e;
        unsigned __int128 new_d = curr_d;
        /* Multiply by p^e */
        for (uint64_t k = 0; k < e; ++k) {
            new_d *= (unsigned __int128)pr[idx];
        }
        gen_divisors(pr, ex, num, idx + 1, new_d, exp_curr, n, g_acc);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for out‑of‑range, not used by OEIS offset */

    /* Factorise n. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor(n, pr, ex, &cnt);

    /* Compute rad(n) as product of distinct primes. */
    unsigned __int128 rad = 1;
    for (size_t i = 0; i < cnt; ++i) {
        rad *= (unsigned __int128)pr[i];
    }

    /* Compute f(n) = sum e_i * (n / p_i). */
    unsigned __int128 f_n = 0;
    for (size_t i = 0; i < cnt; ++i) {
        f_n += (unsigned __int128)ex[i] * ((unsigned __int128)n / (unsigned __int128)pr[i]);
    }

    unsigned __int128 n128 = (unsigned __int128)n;

    /* Compute g(n) = Σ_{d|n} f(d) * (n/d). */
    unsigned __int128 g = 0;
    int exp_curr[64] = {0};
    gen_divisors(pr, ex, cnt, 0, (unsigned __int128)1, exp_curr, n128, &g);

    /* denom = n / rad */
    unsigned __int128 denom = n128 / rad;

    /* Final result: gcd(f_n, g) / denom */
    unsigned __int128 gval = gcd128(f_n, g);
    unsigned __int128 result = gval / denom;

    return (seq_t)result;
}
