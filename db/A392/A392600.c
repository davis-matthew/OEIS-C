/*
 * A392600  a(n) = A327860(n) mod 360, where A327860 is the arithmetic
 *          derivative of the primorial base exp-function.
 * https://oeis.org/A392600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A327860(n) mod 360
 * Range:     u8 n <= 10, u16 n <= 8473, u32 n <= 8473, u64 n <= 8473,
 *            u128 n <= 8473, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A327860(n) is the arithmetic derivative of the primorial base exp-function.
   The function is implemented by first converting n into its primorial number
   system representation.  The digits are used as exponents of the primes,
   producing the value E(n).  The arithmetic derivative of E(n) is
   E(n) * Σ (e_i / p_i), where e_i are the exponents.  The result is reduced
   modulo 360.  All intermediate arithmetic uses only integer operations.    */

#include <stddef.h>

/* primes needed for primorial base representation up to n <= 10,000
   (enough for weights up to 30,000; extend if larger n is required). */
static const idx_t _small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101};
static const size_t _PRIME_CNT = sizeof(_small_primes) / sizeof(_small_primes[0]);

/* fast modular exponentiation for seq_t with a constant modulus */
static seq_t modpow_seq_t(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    const seq_t MOD = 360;

    /* Handle n = 0 directly */
    if (n == 0)
        return 0;

    /* Compute weights (primorials) and determine the number of positions */
    idx_t weight = 1;              /* weight[1] = 1 */
    size_t max_k = 0;
    idx_t weights[32];             /* enough to cover all positions for n <= 10,000 */
    while (weight <= n && max_k < 31)
    {
        ++max_k;
        weights[max_k] = weight;  /* store current weight */

        /* Prepare next weight: multiply by the corresponding prime.
           For weight[max_k+1] we need primes[max_k-1] (since prime[0] for position 2). */
        if (max_k < _PRIME_CNT)
            weight = weight * _small_primes[max_k - 1];
        else
            break;
    }

    /* Decompose n in primorial base to obtain exponents a[1..max_k] */
    idx_t exps[32] = {0};
    idx_t remaining = n;
    for (size_t i = max_k; i >= 1; --i)
    {
        exps[i] = remaining / weights[i];
        remaining -= exps[i] * weights[i];
    }

    /* Compute E(n) modulo MOD and store as E_mod for use in derivative */
    seq_t E_mod = 1 % MOD;
    for (size_t i = 1; i <= max_k; ++i)
    {
        if (exps[i] == 0)
            continue;
        E_mod = (E_mod * modpow_seq_t(_small_primes[i - 1], exps[i], MOD)) % MOD;
    }

    /* Compute the arithmetic derivative modulo MOD */
    seq_t sum = 0;
    for (size_t i = 1; i <= max_k; ++i)
    {
        if (exps[i] == 0)
            continue;                       /* contributes nothing */

        /* Compute E(n) / prime_i modulo MOD */
        seq_t prod = 1 % MOD;
        for (size_t j = 1; j <= max_k; ++j)
        {
            idx_t e = exps[j];
            if (j == i)
                e -= 1;
            if (e == 0)
                continue;
            prod = (prod * modpow_seq_t(_small_primes[j - 1], e, MOD)) % MOD;
        }

        /* Term = exponent * (E / prime) */
        seq_t term = (exps[i] * prod) % MOD;
        sum += term;
        sum %= MOD;
    }

    return sum % MOD;
}
