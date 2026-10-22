/*
 * A328902  Triangle T(n, k) read by rows: T(n, k) is the denominator of the
 *          rational Catalan number defined as binomial(n + k, n)/(n + k) for
 *          0 <= k <= n, n > 0; T(0, 0) = 1.
 * https://oeis.org/A328902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn, tabl
 * Formula:   a(n) enumerates the denominators T(n, k) =
 *            (n+k)/gcd(binomial(n+k, n), n+k) row‑by‑row (offset 0)
 * Range:     u8 n <= 9328, u16 n <= 9328, u32 n <= 9328, u64 n <= 9328,
 *            u128 n <= 9328, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* exponent of prime p in n! */
static idx_t exp_in_fact(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* check if x is prime (x ≥ 2) */
static bool is_prime_small(idx_t x)
{
    if (x < 2) return false;
    for (idx_t d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* find the row r: (r*(r+1))/2 <= n < ((r+1)*(r+2))/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    idx_t offset = (r * (r + 1)) / 2;   /* starting index of this row */
    idx_t k = n - offset;               /* column index within row */
    idx_t t = r + k;                    /* t = n + k */

    seq_t denom = (seq_t)t;             /* start with t */

    /* factorise t and apply gcd with binomial coefficient */
    for (idx_t p = 2; p <= t; ++p) {
        if (!is_prime_small(p))
            continue;

        /* exponent of p in t */
        idx_t tmp = t;
        int exp_t = 0;
        while (tmp % p == 0) {
            ++exp_t;
            tmp /= p;
        }
        if (exp_t == 0)
            continue;

        /* exponent of p in binomial(t, r) using Legendre's formula */
        idx_t exp_bin = exp_in_fact(t, p) - exp_in_fact(r, p) - exp_in_fact(t - r, p);
        int exp_g = exp_t < exp_bin ? exp_t : exp_bin;

        /* divide denom by p^exp_g */
        for (int i = 0; i < exp_g; ++i)
            denom /= (seq_t)p;
    }

    return denom;
}
