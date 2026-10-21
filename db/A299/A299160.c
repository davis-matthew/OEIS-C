/*
 * A299160  In factorial base, any rational number has a terminating
 *          expansion; hence we can devise a self-inverse permutation of the
 *          rational numbers, say f, such that for any rational number q, the
 *          representations of q and of f(q) in factorial base are mirrored
 *          around the radix point and q and f(q) have th...
 * https://oeis.org/A299160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(n) = denominator of Σ_{i=1}^k d_i/(i+1)! , where (d_k … d_1)
 *            are the factorial‑base digits of n and k is the length of the
 *            representation (i.e., the largest i with i! ≤ n). This
 *            implements the definition of A299160
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Pre‑computed factorials up to 21! (fits in uint64_t).  */
static const idx_t fact[22] = {
    1ULL,          /* 0! */
    1ULL,          /* 1! */
    2ULL,          /* 2! */
    6ULL,          /* 3! */
    24ULL,         /* 4! */
    120ULL,        /* 5! */
    720ULL,        /* 6! */
    5040ULL,       /* 7! */
    40320ULL,      /* 8! */
    362880ULL,     /* 9! */
    3628800ULL,    /* 10! */
    39916800ULL,   /* 11! */
    479001600ULL,  /* 12! */
    6227020800ULL, /* 13! */
    87178291200ULL,/* 14! */
    1307674368000ULL, /* 15! */
    20922789888000ULL,/* 16! */
    355687428096000ULL,/* 17! */
    6402373705728000ULL,/* 18! */
    121645100408832000ULL,/* 19! */
    2432902008176640000ULL,/* 20! */
    0ULL                  /* placeholder for 21! overflow (unused) */
};

/* Compute A299160(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* Determine k: the largest i with i! <= n */
    idx_t k = 0;
    while (k + 1 <= 20 && fact[k + 1] <= n) {
        ++k;
    }

    /* Factorial‑base digits d[1..k], d[1] is the 1! coefficient */
    idx_t d[22] = {0};
    idx_t remaining = n;
    for (idx_t i = k; i >= 1; --i) {
        d[i] = remaining / fact[i];
        remaining -= d[i] * fact[i];
    }

    /* Compute (k+1)! as seq_t */
    seq_t M = (seq_t)fact[k + 1];

    /* Compute N = Σ d[i] * (M / fact[i+1]) */
    seq_t N = (seq_t)0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t mult = M / (seq_t)fact[i + 1];      /* (k+1)! / (i+1)!  */
        seq_t term = mult * (seq_t)d[i];
        N += term;
    }

    /* Denominator = M / gcd(N, M) */
    seq_t g = gcd(N, M);
    seq_t denom = M / g;
    return denom;
}
