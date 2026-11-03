/*
 * A336917  Number of iterations of x -> A252463(x) needed before the result
 *          is deficient, when starting from x=n.
 * https://oeis.org/A336917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 971, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9814 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336917: iterate x → A252463(x) (x even → x/2, x odd → product+sum of
 * distinct prime factors) until the result is deficient (σ(x) < 2x).  Return the
 * number of iterations performed. */
#include <stdbool.h>
#include <stdint.h>

/* Compute A252463(x) for odd x: product + sum of distinct prime factors. */
static inline idx_t odd_map(idx_t x)
{
    idx_t t = x;
    idx_t prod = 1;
    idx_t sum  = 0;
    for (idx_t p = 3; p <= t / p; p += 2)
        if (t % p == 0) {
            prod *= p;
            sum  += p;
            while (t % p == 0) t /= p;
        }
    if (t > 1) {                 /* remaining prime factor */
        prod *= t;
        sum  += t;
    }
    return prod + sum;
}

/* Sum of all divisors of n.  n ≤ 2*max_input ≈ 20000 for the intended test
 * limits, so a simple trial‑division works fast. */
static inline seq_t sigma_of_val(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 1;                /* divisor 1 */
    for (idx_t d = 2; d <= n / d; ++d)
        if (n % d == 0) {
            sum += d;
            if (d != n / d) sum += n / d;
        }
    if (n > 1) sum += n;          /* add n itself (not for n = 1) */
    return sum;
}

/* Return true if n is non‑deficient: σ(n) ≥ 2n. */
static inline bool is_nondeficient(idx_t n)
{
    return sigma_of_val(n) >= (seq_t)2 * n;
}

/* Main sequence function.  Count iterations until a deficient number is
 * produced. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t cur   = n;

    while (is_nondeficient(cur)) {
        /* map according to parity of cur */
        if ((cur & 1ULL) == 0)        /* even case: n/2 */
            cur >>= 1;
        else                          /* odd case: product+sum of distinct primes */
            cur = odd_map(cur);
        ++count;
    }
    return count;
}
