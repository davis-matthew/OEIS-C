/*
 * A396903  a(n) = Sum_{k=1..n} gcd(k^4,n^3).
 * https://oeis.org/A396903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{k=1..n} gcd(k^4,n^3)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Compute a(p^e) for a prime p and exponent e >= 1 */
static seq_t a_prime_power(uint64_t p, int e)
{
    /* precompute p^i for i = 0..3*e */
    seq_t p_pow[256];                    /* 3*e ≤ 192 for e ≤ 64 */
    p_pow[0] = (seq_t)1;
    for (int i = 1; i <= 3 * e; ++i)
        p_pow[i] = p_pow[i - 1] * (seq_t)p;

    seq_t sum = p_pow[3 * e];             /* p^(3e) */
    for (int k = 0; k < e; ++k) {
        seq_t diff = p_pow[e - k] - p_pow[e - k - 1];
        int min_exp = (4 * k < 3 * e) ? 4 * k : 3 * e;
        sum += diff * p_pow[min_exp];
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is 1-indexed; behaviour for n == 0 is irrelevant */
    if (n == 0) return (seq_t)0;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    size_t cnt = 0;

    /* factor 2 separately */
    if ((nn & 1ULL) == 0) {
        int e = 0;
        while ((nn & 1ULL) == 0) { nn >>= 1; ++e; }
        primes[cnt] = 2;
        exps[cnt]   = e;
        ++cnt;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= nn / p; p += 2) {
        if (nn % p == 0) {
            int e = 0;
            while (nn % p == 0) { nn /= p; ++e; }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (nn > 1) {                /* remaining prime */
        primes[cnt] = nn;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* multiply contributions of each prime power */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        result *= a_prime_power(primes[i], exps[i]);

    return result;
}
