/*
 * A349174  Odd numbers k for which gcd(k, A003961(k)) is equal to
 *          gcd(sigma(k), A003961(k)), where A003961(n) is fully
 *          multiplicative with a(prime(k)) = prime(k+1), and sigma is the sum
 *          of divisors function.
 * https://oeis.org/A349174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th odd integer k for which gcd(k, A003961(k)) ==
 *            gcd(sigma(k), A003961(k))
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   build_spf, gcd, next_prime, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/next_prime.h"
#include "../utils/build_spf.h"

/* ------------------------------------------------------------------ */
/*  Global state                                                     */
/* ------------------------------------------------------------------ */
static idx_t *spf = NULL;          /* smallest prime factor array               */
static idx_t spf_limit = 0;        /* current sieve upper bound                 */

static seq_t *seq_buf = NULL;      /* collected sequence values                */
static size_t seq_cap = 0;         /* allocated size for seq_buf               */
static size_t seq_len = 0;         /* number of terms stored                   */

static idx_t last_checked = 0;     /* last odd number processed                */

/* ------------------------------------------------------------------ */
/*  Ensure SPF array covers all numbers up to limit.                  */
/* ------------------------------------------------------------------ */
static void ensure_spf(idx_t limit)
{
    if (spf_limit >= limit)
        return;

    /* Resize and rebuild the SPF table. */
    if (spf)
        free(spf);
    build_spf(limit, &spf);        /* allocates spf array of size limit+1 */
    spf_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Return the smallest prime > x.                                    */
/* ------------------------------------------------------------------ */
static seq_t next_prime_of(seq_t x)
{
    uint64_t v = (uint64_t)x + 1;
    v = next_prime(v);
    return (seq_t)v;
}

/* ------------------------------------------------------------------ */
/*  Test whether an odd integer k satisfies the defining gcd condition.  */
/* ------------------------------------------------------------------ */
static bool is_valid_k(idx_t k)
{
    if ((k & 1ULL) == 0)            /* only odd numbers are considered */
        return false;

    /* Factor k using the SPF array. */
    idx_t tmp = k;
    idx_t primes[64];
    int   exps[64];
    size_t cnt = 0;

    while (tmp > 1) {
        idx_t p = spf[tmp];
        int   e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[cnt] = p;
        exps[cnt]   = e;
        ++cnt;
    }

    /* Compute A003961(k): product of next_prime(p)^e. */
    seq_t f = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t np  = next_prime_of((seq_t)primes[i]);
        seq_t pw  = seq_pow(np, (idx_t)exps[i]);
        f = seq_mul(f, pw);
    }

    /* Compute sigma(k): product of (p^(e+1)-1)/(p-1). */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p_seq = (seq_t)primes[i];
        seq_t term  = (seq_pow(p_seq, (idx_t)(exps[i] + 1)) - 1) / (p_seq - 1);
        sigma = seq_mul(sigma, term);
    }

    seq_t g1 = gcd((seq_t)k, f);
    seq_t g2 = gcd(sigma, f);
    return g1 == g2;
}

/* ------------------------------------------------------------------ */
/*  Generate terms until we have at least n of them.                    */
/* ------------------------------------------------------------------ */
static void ensure_terms(idx_t n)
{
    if (seq_len >= n)
        return;

    while (seq_len < n) {
        /* Pick the next odd number to test. */
        idx_t k = (last_checked ? last_checked + 2 : 1);

        /* Ensure SPF table covers k. */
        ensure_spf(k);

        if (is_valid_k(k)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 8;
                seq_buf = (seq_t *)realloc(seq_buf, seq_cap * sizeof(seq_t));
            }
            seq_buf[seq_len++] = (seq_t)k;
        }

        last_checked = k;
    }
}

/* ------------------------------------------------------------------ */
/*  Main entry: return the n-th term of the sequence                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n = 0, guard it */

    ensure_terms(n);
    return seq_buf[n - 1];
}
