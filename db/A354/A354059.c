/*
 * A354059  Square array read by ascending antidiagonals: T(n,k) is the number
 *          of elements in the multiplicative group of integers modulo n that
 *          have order k.
 * https://oeis.org/A354059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of elements of order k in (Z/nZ)× read by
 *            ascending antidiagonals. For the pair (n,k) corresponding to
 *            index n we have a(n) = Σ_{d|k} μ(k/d) * t_d, where t_d =
 *            |{x∈(Z/nZ)× : xᵈ≡1 (mod n)}|
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   factor_u64, gcd, mu_of, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/mu_of.h"
#include "../utils/pow_uint.h"
#include "../utils/gcd.h"

/* Count the number of solutions of x^d = 1 in (Z/rZ)×. */
static seq_t tn_dividing(idx_t r, idx_t d)
{
    if (r == 1) return 1;          /* only the identity element */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)r, primes, exps, &cnt);

    seq_t total = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t comp;
        if (p == 2) {
            if (e == 1) {
                comp = 1;
            } else if (e == 2) {
                comp = gcd((seq_t)d, (seq_t)2);
            } else {
                seq_t c1 = 1 + (d % 2 == 0 ? 1 : 0);
                seq_t pow2 = ((seq_t)1) << (e - 2);   /* 2^(e-2) */
                seq_t c2 = gcd((seq_t)d, pow2);
                comp = c1 * c2;
            }
        } else {
            seq_t phi = pow_uint(p, e - 1) * (seq_t)(p - 1); /* φ(p^e) */
            comp = gcd((seq_t)d, phi);
        }
        total *= comp;
    }
    return total;
}

/* Compute T(r,k) = number of elements of order k in (Z/rZ)×. */
static seq_t T(idx_t r, idx_t k)
{
    int64_t sum = 0;
    idx_t kk = k;
    for (idx_t d = 1; d * d <= kk; ++d) {
        if (kk % d == 0) {
            seq_t t_d1 = tn_dividing(r, d);
            int mu1 = mu_of(kk / d);
            sum += (int64_t)mu1 * (int64_t)t_d1;
            if (d * d != kk) {
                seq_t t_d2 = tn_dividing(r, kk / d);
                int mu2 = mu_of(d);
                sum += (int64_t)mu2 * (int64_t)t_d2;
            }
        }
    }
    return (seq_t)sum;
}

/* Public function: a(n) returns the n-th term of A354059. */
seq_t a(idx_t n)
{
    /* Find the antidiagonal sum s such that n ≤ (s‑1)*s/2. */
    idx_t s = 2;
    while ((s - 1) * s / 2 < n) ++s;

    /* t is the position within that diagonal (1…s‑1). */
    idx_t prev_tri = (s - 2) * (s - 1) / 2;
    idx_t t = n - prev_tri;     /* t ∈ [1, s‑1] */

    idx_t row = s - t;          /* n in the square array */
    idx_t col = t;              /* k in the square array */

    return T(row, col);
}
