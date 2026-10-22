/*
 * A318676  Sum of divisors of n that have an even number of prime factors
 *          (counted with multiplicity).
 * https://oeis.org/A318676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * [Ω(d) even] (Ω(d) = total number of prime
 *            factors of d, counted with multiplicity)
 * Range:     u8 n <= 195, u16 n <= 9491, u32 n <= 9491, u64 n <= 9491,
 *            u128 n <= 9491, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Factorise n into primes and exponents (simple trial division) */
static void factor_u64_my(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t i = 0;
    if (n < 2) {
        *cnt = 0;
        return;
    }
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[i]   = p;
            ex[i]   = e;
            ++i;
        }
    }
    if (n > 1) {
        pr[i] = n;
        ex[i] = 1;
        ++i;
    }
    *cnt = i;
}

/* Recursive enumeration of divisors, accumulating those with even Ω(d) */
static void add_divisors(seq_t *sum,
                         size_t idx,
                         seq_t cur,
                         unsigned omega,
                         const uint64_t *primes,
                         const uint64_t *exps,
                         size_t cnt)
{
    if (idx == cnt) {
        if ((omega & 1) == 0)   /* even number of prime factors */
            *sum += cur;
        return;
    }
    seq_t prod = cur;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        add_divisors(sum, idx + 1, prod, omega + (unsigned)e, primes, exps, cnt);
        prod *= (seq_t)primes[idx];   /* prepare next exponent */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)        /* values below offset not defined; return 0 */
        return 0;
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64_my(nn, primes, exps, &cnt);

    seq_t sum = 0;
    add_divisors(&sum, 0, (seq_t)1, 0, primes, exps, cnt);
    return sum;
}
