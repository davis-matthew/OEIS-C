/*
 * A371144  The smallest number such that the concatenation of n, a(n), n+1 is
 *          divisible by the concatenation of n and n+1.
 * https://oeis.org/A371144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 908, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits, modinv64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the smallest non‑negative integer a such that the concatenation of n,
   a, n+1 is divisible by the concatenation of n and n+1. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digits.h"
#include "../utils/modinv64.h"

/* 10^e for 0 ≤ e ≤ 20 (fits in uint64_t) */
static inline uint64_t pow10_u64(unsigned int e) {
    uint64_t r = 1ULL;
    while (e--) r *= 10ULL;
    return r;
}

/* Euclidean GCD for uint64_t */
static inline uint64_t gcd64(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    uint64_t n1  = n64 + 1ULL;
    int len_n1 = digits(n1);

    uint64_t power   = pow10_u64((unsigned int)len_n1);        /* 10^digits(n+1) */
    uint64_t B       = n64 * power + n1;                      /* concat(n, n+1) */
    uint64_t amax    = n1 * pow10_u64((unsigned int)digits(n64)) + n64;  /* concat(n+1, n) */
    int max_k = (int)digits(amax);                            /* maximal digits of a */

    uint64_t best = 0;
    bool found = false;

    for (int k = 1; k <= max_k; ++k) {
        uint64_t pow10k       = pow10_u64((unsigned int)k);
        uint64_t pow10kminus1 = (k == 1 ? 0ULL : pow10_u64((unsigned int)(k - 1)));
        uint64_t L = (k == 1 ? 0ULL : pow10kminus1);          /* lower bound for a of length k */
        uint64_t U = pow10k - 1ULL;                            /* upper bound for a of length k */

        uint64_t c = n1 * (pow10k - 1ULL);                     /* (n+1)(10^k-1) */

        uint64_t g = gcd64(power, B);
        if (c % g != 0) continue;                              /* no solution for this k */

        uint64_t Bp     = B / g;
        uint64_t powerp = power / g;
        uint64_t cp     = c / g;

        uint64_t inv = modinv64(powerp, Bp);
        if (inv == 0) continue;                                /* should not happen */

        uint64_t a0 = (cp % Bp) * inv % Bp;                    /* smallest solution modulo B' */

        uint64_t candidate = a0;
        if (candidate < L) {
            uint64_t diff = L - candidate;
            uint64_t t_min = (diff + Bp - 1ULL) / Bp;          /* ceil((L - a0)/Bp) */
            candidate = candidate + t_min * Bp;
        }
        if (candidate > U) continue;                           /* lies outside digit length */

        if (!found || candidate < best) {
            best  = candidate;
            found = true;
            if (best == 0) break;                              /* minimal possible */
        }
    }

    return (seq_t)best;
}
