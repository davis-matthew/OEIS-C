/*
 * A300251  Möbius transform of arithmetic derivative (A003415).
 * https://oeis.org/A300251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Σ_{d|n} μ(d) * A003415(n/d)
 * Range:     u8 n <= 127, u16 n <= 9789, u32 n <= 9789, u64 n <= 9789,
 *            u128 n <= 9789, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A003415 is the arithmetic derivative
 * and μ is the Möbius function.
 *
 * The function uses a direct computation of the sum over square‑free
 * divisors of n.  It is efficient for n ≤ 10 000 and works for all
 * supported seq_t sizes. */

#include <stdint.h>
#include <stddef.h>

/* integer power for small exponents; sequence type is seq_t. */
static seq_t pow_seq_t(seq_t base, int exp)
{
    seq_t r = 1;
    while (exp > 0) {
        if (exp & 1) r *= base;
        base *= base;
        exp >>= 1;
    }
    return r;
}

/* arithmetic derivative of an integer described by its prime
 * factorisation (prime[i]^exp[i]),  i = 0..cnt-1.  */
static seq_t arith_deriv_prime(const uint64_t primes[], const int exps[], int cnt)
{
    /* value = product p_i^e_i */
    seq_t value = 1;
    for (int i = 0; i < cnt; ++i) value *= pow_seq_t((seq_t)primes[i], exps[i]);

    seq_t der = 0;
    for (int i = 0; i < cnt; ++i) {
        int e = exps[i];
        if (e == 0) continue;
        /* term = e * p^(e-1) * (value / p^e)  = e * value / p */
        der += (seq_t)e * (value / (seq_t)primes[i]);
    }
    return der;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset = 1 */

    /* factorise n (n ≤ 10 000 → at most 8 distinct primes). */
    uint64_t primes[16];
    int exps[16];
    int cnt = 0;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* Accumulate positive and negative contributions separately. */
    seq_t pos = 0, neg = 0;

    int totalMasks = 1 << cnt;            /* all subsets of distinct primes */
    for (int mask = 0; mask < totalMasks; ++mask) {
        /* compute exponents for n/d where d corresponds to the subset (mask) */
        int exp_prime[16];
        for (int i = 0; i < cnt; ++i)
            exp_prime[i] = exps[i] - ((mask >> i) & 1);

        /* derivative of n/d */
        seq_t der = arith_deriv_prime(primes, exp_prime, cnt);

        /* μ(d) = (-1)^{#primes in d} because d is square‑free here. */
        int bits = __builtin_popcount((unsigned)mask);
        if ((bits & 1) == 0)
            pos += der;
        else
            neg += der;
    }

    /* a(n) = pos - neg; it is guaranteed non‑negative. */
    return pos - neg;
}
