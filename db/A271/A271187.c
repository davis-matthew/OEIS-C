/*
 * A271187  Triangle T(n,k) read by rows: T(n,k) is the squarefree part of
 *          C(n,k).
 * https://oeis.org/A271187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = squarefree part of binomial coefficient C(r,k) where
 *            (r,k) correspond to the n-th element in the triangle T(n,k) read
 *            row‑by‑row: (0,0),(1,0),(1,1),(2,0),(2,1),(2,2),
 * Range:     u8 n <= 69, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   _floor_isqrt_u64, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/build_primes.h"
#include "../utils/_floor_isqrt_u64.h"   /* if named the same; adjust as needed */

/* Compute exponent of prime p in n! */
static idx_t vp_factorial(idx_t n, idx_t p) {
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

seq_t a(idx_t n) {
    /* find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = (_floor_isqrt_u64(8 * n + 1) - 1) / 2;
    idx_t k = n - r * (r + 1) / 2;

    /* C(r,k) = 1 for k == 0 or k == r */
    if (k == 0 || k == r) return (seq_t)1;

    /* get primes up to r */
    static idx_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    if (r > primes_limit) {
        /* enlarge primes list */
        build_primes(r, &primes, &primes_cnt);
        primes_limit = r;
    }

    seq_t res = 1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p > r) break;
        idx_t e = vp_factorial(r, p) - vp_factorial(k, p) - vp_factorial(r - k, p);
        if (e & 1) {
            res *= (seq_t)p;
        }
    }
    return res;
}
