/*
 * A354058  Square array read by ascending antidiagonals: T(n,k) is the number
 *          of degree-k primitive Dirichlet characters modulo n.
 * https://oeis.org/A354058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where (row,col) is the index pair * obtained
 *            by reading the square array of T(n,k) by ascending
 *            antidiagonals: * let s be the smallest integer with s*(s-1)/2 ≥
 *            n; then * count before this antidiagonal is (s-1)*(s-2)/2, and
 *            position inside it * is pos = n - that - 1 (0‑based). T...
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t T_prime_power(uint64_t p, int e, idx_t k)
{
    if (p == 2ULL) {
        if (e == 1) return 0;                /* T(2,k) = 0 */
        if (e == 2) return (k % 2 == 0) ? 1 : 0;  /* T(4,k) as described */
        /* e >= 3: T(2^e,k) = 0 unless k is divisible by 2^(e-2), equal to that power */
        uint64_t div = 1ULL << (e - 2);
        return (k % div == 0) ? (seq_t)div : 0;
    }
    /* odd prime p */
    seq_t part;
    if (e == 1) { /* T(p,k) = gcd(p-1,k)-1 */
        part = gcd((seq_t)(p - 1), (seq_t)k);
        return part - 1;
    }
    /* e >= 2: k must be multiple of p^(e-1) */
    uint64_t pk_1 = 1;
    for (int i = 0; i < e - 1; ++i) pk_1 *= p;
    if (k % pk_1 != 0) return 0;
    uint64_t m = k / pk_1;
    seq_t pow_p_e_2 = 1;
    for (int i = 0; i < e - 2; ++i) pow_p_e_2 *= p;
    seq_t g = gcd((seq_t)m, (seq_t)(p - 1));
    part = pow_p_e_2 * (seq_t)(p - 1) * g;
    return part;
}

static seq_t T_of_n_k(idx_t n, idx_t k)
{
    if (n == 1ULL) return 1;   /* T(1,k) = 1 for all k */
    if (n == 2ULL) return 0;   /* T(2,k) = 0 for all k */
    seq_t result = 1;
    uint64_t nn = (uint64_t)n;
    uint64_t kk = (uint64_t)k;
    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            int e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            seq_t part = T_prime_power(p, e, kk);
            if (part == 0) return 0;
            result *= part;
        }
    }
    if (nn > 1) {            /* remaining prime factor with exponent 1 */
        seq_t part = T_prime_power(nn, 1, kk);
        if (part == 0) return 0;
        result *= part;
    }
    return result;
}

seq_t a(idx_t N)
{
    /* N is 1‑based sequence index. */
    if (N == 0) return 0;   /* undefined, but guard anyway */

    idx_t s = 2;
    while ((s * (s - 1)) / 2 < N) ++s;

    idx_t cum_before = ((s - 1) * (s - 2)) / 2;
    idx_t pos = N - cum_before - 1;                /* 0‑based within antidiagonal */

    idx_t row = (s - 1) - pos;                     /* modulus */
    idx_t col = s - row;                           /* degree k */

    return T_of_n_k(row, col);
}
