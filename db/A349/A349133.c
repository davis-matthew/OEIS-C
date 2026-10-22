/*
 * A349133  Dirichlet convolution of A003415 with A003958, where A003415 is
 *          the arithmetic derivative and A003958 is fully multiplicative with
 *          a(p) = (p-1).
 * https://oeis.org/A349133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A003415(d) * A003958(n/d) where A003415 is the
 *            arithmetic derivative and A003958 is fully multiplicative with
 *            A003958(p) = p-1
 * Range:     u8 n <= 47, u16 n <= 2879, u32 n <= 9097, u64 n <= 9097,
 *            u128 n <= 9097, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper to factor a 64‑bit integer.
   primes[] and exps[] must be large enough (up to 64 entries).      */
static void factor_uint64(uint64_t n, uint64_t primes[], int exps[], size_t *cnt)
{
    size_t i = 0;
    if (n < 2) { *cnt = 0; return; }

    while ((n & 1ULL) == 0ULL) {
        primes[i]   = 2ULL;
        exps[i]     = 0;                 /* count occurrences */
        while ((n & 1ULL) == 0ULL) {
            exps[i]++;
            n >>= 1;
        }
        i++;
    }

    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0ULL) {
            primes[i]   = p;
            exps[i]     = 0;
            while (n % p == 0ULL) {
                exps[i]++;
                n /= p;
            }
            i++;
        }
    }
    if (n > 1ULL) {
        primes[i]   = n;
        exps[i]     = 1;
        i++;
    }
    *cnt = i;
}

/* Recursive enumeration of divisors, accumulating the required sum. */
static void dfs(const uint64_t primes[], const int exps[], size_t cnt,
                size_t idx, seq_t d_cur, seq_t der_cur, seq_t f_cur,
                seq_t *acc)
{
    if (idx == cnt) {
        /* All primes processed: add contribution of this divisor. */
        *acc += der_cur * f_cur;
        return;
    }

    /* Pre‑compute powers of the current prime and of (p-1).              */
    size_t a = exps[idx];
    seq_t powp[65];          /* powp[e] = p^e */
    seq_t powpm1[65];        /* powpm1[e] = (p-1)^e */
    seq_t p = (seq_t)primes[idx];
    seq_t p_minus_1 = p - (seq_t)1ULL;

    powp[0] = 1;
    powpm1[0] = 1;
    for (size_t e = 1; e <= a; ++e) {
        powp[e]   = powp[e-1] * p;
        powpm1[e] = powpm1[e-1] * p_minus_1;
    }

    for (size_t e = 0; e <= a; ++e) {
        /* New divisor value. */
        seq_t d_new = d_cur * powp[e];

        /* New derivative.  Derivative of a* b where b=p^e */
        seq_t der_new;
        if (e == 0) {
            der_new = der_cur;            /* multiply by 1 */
        } else {
            /* derivative of p^e is e * p^{e-1}                      */
            seq_t term = (seq_t)e * powp[e-1];
            der_new = d_cur * term + der_cur * powp[e];
        }

        /* f(n/d) = product over primes of (p-1)^{remaining exponent} */
        seq_t f_next = f_cur * powpm1[a - e];

        dfs(primes, exps, cnt, idx + 1, d_new, der_new, f_next, acc);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */

    uint64_t primes[64];
    int exps[64];
    size_t cnt;
    factor_uint64(n, primes, exps, &cnt);

    seq_t acc = 0;
    /* initial divisor d=1, derivative=0, f(n/1)=1 */
    dfs(primes, exps, cnt, 0, (seq_t)1, (seq_t)0, (seq_t)1, &acc);
    return acc;
}
