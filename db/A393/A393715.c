/*
 * A393715  a(n) = number of triples (x, y, z) such that x^2 + y*z = n, where
 *          x, y, z are positive integers with gcd(x, y*z) > 1.
 * https://oeis.org/A393715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A393715: a(n) = number of triples (x,y,z) of positive integers
 * with x^2 + y*z = n and gcd(x, y*z) > 1.
 *
 * Since y*z = m, the condition is equivalent to gcd(x, m) > 1.
 * For each x with x^2 < n, let m = n - x^2 > 0.
 * Count all ordered factor pairs (y,z) of m, which is τ(m), the number
 * of positive divisors of m.  Thus
 *     a(n) = Σ_{x=1}^{⌊√(n-1)⌋} τ(n - x^2) * [gcd(x, n - x^2) > 1].
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square‑root for unsigned 64‑bit values. */
static inline idx_t
isqrt_u64(idx_t n)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 62;               /* largest power of 2 ≤ √(2^64) */
    while (bit) {
        idx_t nxt = r | bit;
        if ((__uint128_t)nxt * nxt <= n)
            r = nxt;
        bit >>= 1;
    }
    return r;
}

/* Euclidean GCD for 64‑bit unsigned integers. */
static inline idx_t
gcd_u64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Cache of divisor counts (τ array).  tau[i] holds the number of
 * divisors of i as a seq_t value. */
static seq_t *tau = NULL;
static idx_t  tau_cap = 0;

/* Ensure tau array is allocated and filled up to n (inclusive). */
static void
ensure_tau(idx_t n)
{
    if (n <= tau_cap)
        return;

    /* Allocate new table. */
    seq_t *new_tau = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!new_tau)
        return;                      /* allocation failure → leave old table */

    /* Compute number of divisors for all integers up to n. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i)
            ++new_tau[j];
    }

    /* Replace old table. */
    free(tau);
    tau = new_tau;
    tau_cap = n;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset 0 */

    /* Pre‑compute divisor counts up to n. */
    ensure_tau(n);

    seq_t result = 0;
    idx_t max_x = isqrt_u64(n - 1);    /* largest x with x^2 < n */

    for (idx_t x = 1; x <= max_x; ++x) {
        idx_t m = n - x * x;           /* m > 0 by construction */
        if (gcd_u64(x, m) > 1)
            result += tau[m];
    }
    return result;
}
