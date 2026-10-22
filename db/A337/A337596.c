/*
 * A337596  Largest m such that k^n (mod m) is always either 0, +1, or -1.
 * https://oeis.org/A337596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Largest m such that k^n (mod m) is always either 0, +1, or -1.
 *            The algorithm: 1. n==1 -> return 3. 2. For each odd prime p with
 *            p-1 | 2n, the largest odd prime power that can appear is p^e
 *            where e = 1 + v_p( (2n)/(p-1) ). Take the maximum of all such
 *            prime powers → candidate1. 3. A power of two 2...
 * Range:     u8 n <= 63, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count trailing zeros of a 64‑bit integer */
static inline idx_t trailing_zeros(idx_t x)
{
    idx_t cnt = 0;
    while ((x & 1) == 0 && x != 0) {
        ++cnt;
        x >>= 1;
    }
    return cnt;
}

/* Simple deterministic primality test for numbers < 65 536.
   n is small (≤ 20001) in this algorithm, so trial division is fine. */
static inline bool is_prime_small(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Special case n == 1 (a(1) = 3) */
    if (n == 1) return (seq_t)3;

    seq_t best = 1;           /* will become at least 3 */

    /* 2 * n -------------------------------------------------------- */
    idx_t two_n = (idx_t)2 * n;

    /* Enumerate all divisors d of 2n.
       For each divisor, we consider the prime p = d + 1. */
    for (idx_t i = 1; i * i <= two_n; ++i) {
        if (two_n % i != 0) continue;

        /* First divisor d1 = i */
        idx_t d1 = i;
        if (d1 > 1) {            /* p = d1 + 1 >= 3 */
            idx_t p = d1 + 1;
            if (is_prime_small(p)) {
                /* t = 2n / (p-1) = two_n / d1 */
                idx_t t = two_n / d1;
                idx_t e = 1;
                idx_t tmp = t;
                while (tmp % p == 0) { ++e; tmp /= p; }

                /* Compute p^e */
                seq_t pow = (seq_t)1;
                for (idx_t k = 0; k < e; ++k) pow *= (seq_t)p;
                if (pow > best) best = pow;
            }
        }

        /* Second divisor d2 = two_n / i, if distinct */
        idx_t d2 = two_n / i;
        if (d2 == d1) continue;  /* already processed */
        if (d2 > 1) {
            idx_t p = d2 + 1;
            if (is_prime_small(p)) {
                idx_t t = two_n / d2;
                idx_t e = 1;
                idx_t tmp = t;
                while (tmp % p == 0) { ++e; tmp /= p; }

                seq_t pow = (seq_t)1;
                for (idx_t k = 0; k < e; ++k) pow *= (seq_t)p;
                if (pow > best) best = pow;
            }
        }
    }

    /* Power of two candidate ------------------------------------ */
    idx_t v2 = trailing_zeros(n);          /* val2(n) */
    idx_t exp = v2 + 2;                    /* from φ(2^a) | 2n */
    if (exp > n) exp = n;                  /* also require 2^exp | 0 (even k) */
    seq_t pow2 = (seq_t)1;
    for (idx_t k = 0; k < exp; ++k) pow2 <<= 1;
    if (pow2 > best) best = pow2;

    return best;
}
