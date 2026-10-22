/*
 * A353395  Numbers k such that the prime shadow of k equals the product of
 *          prime shadows of the prime indices of k.
 * https://oeis.org/A353395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k ≥ 1 such that prime_shadow(k) = product
 *            of prime_shadows of its prime indices (each index counted with
 *            its multiplicity)
 * Range:     u8 n <= 46, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

#define MAX_SIEVE       5000000   /* sieve limit: large enough for n <= 10000 */
#define MAX_SEQ_TERMS   20000     /* space for stored terms              */

static uint32_t spf[MAX_SIEVE + 1];           /* smallest prime factor */
static uint32_t primes[MAX_SIEVE + 1];         /* list of primes        */
static uint32_t prime_to_index[MAX_SIEVE + 1];/* prime → 1‑based index  */
static size_t   primes_cnt = 0;               /* number of primes      */

static seq_t seq[MAX_SEQ_TERMS];              /* cached sequence values */
static idx_t  seq_len = 0;                    /* number of terms cached */
static idx_t  next_candidate = 1;             /* next integer to test   */

/* --------------------------------------------------------------------
   Build the sieve once.  The arrays are static so they are zero‑filled.
   -------------------------------------------------------------------- */
static void build_sieve(void)
{
    static bool done = false;
    if (done) return;

    for (uint32_t i = 2; i <= MAX_SIEVE; ++i)
    {
        if (spf[i] == 0)
        {
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j)
        {
            uint32_t p = primes[j];
            uint64_t v = (uint64_t)p * i;
            if (v > MAX_SIEVE) break;
            spf[v] = p;
            if (p == spf[i]) break;
        }
    }
    for (size_t idx = 0; idx < primes_cnt; ++idx)
        prime_to_index[primes[idx]] = (uint32_t)(idx + 1);

    done = true;
}

/* --------------------------------------------------------------------
   Compute the prime shadow of n (product of primes indexed by exponents).
   -------------------------------------------------------------------- */
static seq_t prime_shadow_value(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    idx_t t = n;
    seq_t prod = (seq_t)1;
    while (t > 1)
    {
        uint32_t p = spf[t];
        int exp = 0;
        while (t % p == 0)
        {
            t /= p;
            exp++;
        }
        prod *= (seq_t)primes[exp - 1];
    }
    return prod;
}

/* --------------------------------------------------------------------
   Test whether k satisfies the defining property.
   Each prime factor p^e contributes:
     - ps_k *= prime(e)
     - prod_idx_ps *= (prime_shadow(idx(p)))^e
   -------------------------------------------------------------------- */
static bool satisfies_property(idx_t k)
{
    if (k <= 1) return true; /* 1 is included by definition */

    idx_t t = k;
    seq_t ps_k = (seq_t)1;
    seq_t prod_idx_ps = (seq_t)1;

    while (t > 1)
    {
        uint32_t p = spf[t];
        int exp = 0;
        while (t % p == 0)
        {
            t /= p;
            exp++;
        }
        ps_k *= (seq_t)primes[exp - 1];

        uint32_t idx = prime_to_index[p];
        seq_t ps_idx = prime_shadow_value(idx);
        prod_idx_ps *= seq_pow(ps_idx, exp);
    }
    return ps_k == prod_idx_ps;
}

/* --------------------------------------------------------------------
   Sequence function: return the nth term (1‑based offset).
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, undefined for n = 0 */

    build_sieve();                          /* ensure primes are ready */

    while (seq_len < n)
    {
        if (satisfies_property(next_candidate))
        {
            if (seq_len >= MAX_SEQ_TERMS) abort();
            seq[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq[n - 1];
}
