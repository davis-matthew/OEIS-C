/*
 * A372566  Numbers k such that k, sigma(k) and A003961(k) have a common
 *          divisor larger than 1, where A003961(n) is fully multiplicative
 *          function with a(prime(i)) = prime(i+1).
 * https://oeis.org/A372566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 9635, u32 n <= 9635, u64 n <= 9635,
 *            u128 n <= 9635, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A372566 – Numbers k such that k, σ(k) and A003961(k) have a
   common divisor > 1.
   The function returns the n‑th term; a(1) = 6.
   Implementation: sieve for smallest prime factors up to a fixed
   maximum, then test integers in order and store the ones that pass
   the condition.  All dynamic allocations are cast from malloc()
   to support both C11 and C++17, including builds where seq_t is a
   big‑integer class that is trivially constructible. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"   /* gcd(seq_t a, seq_t b) */

#define LIMIT 5000000          /* should cover the first ~10 000 terms */

/* --- helper functions ------------------------------------------------- */

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d*d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

static seq_t next_prime_of_uint64(uint64_t p)
{
    if (p == 2) return (seq_t)3;
    uint64_t cand = p + 2;
    while (!is_prime_uint64(cand)) cand += 2;
    return (seq_t)cand;
}

static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static uint64_t add_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    uint64_t s = a + b;
    return s >= mod ? s - mod : s;
}

/* --- global sieve data ----------------------------------------------- */

static int  *spf      = NULL;   /* smallest prime factor of i */
static int  *primes   = NULL;   /* list of primes up to LIMIT */
static size_t prime_cnt = 0;

static void build_sieve(void)
{
    int *f  = (int*)malloc((LIMIT + 1) * sizeof(int));
    int *p  = (int*)malloc(LIMIT * sizeof(int));  /* enough for primes */
    size_t pc = 0;
    memset(f, 0, (LIMIT + 1) * sizeof(int));

    for (int i = 2; i <= LIMIT; ++i) {
        if (f[i] == 0) {
            f[i] = i;
            p[pc++] = i;
        }
        for (size_t j = 0; j < pc && p[j] <= f[i] && (int64_t)i * p[j] <= LIMIT; ++j)
            f[i * p[j]] = p[j];
    }

    spf   = f;
    primes = p;
    prime_cnt = pc;
}

/* --- main sequence function -------------------------------------------- */

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static int last_candidate = 2;   /* next integer to test */

    if (!initialized) {
        build_sieve();
        terms_cap = 256;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        terms_len = 0;
        initialized = true;
    }

    while (terms_len < n) {
        int k = last_candidate++;   /* candidate integer */

        /* factorization via spf */
        int rem = k;
        uint64_t prime_list[32];
        int exp_list[32];
        int nf = 0;
        while (rem > 1) {
            int p = spf[rem];
            int e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            prime_list[nf] = (uint64_t)p;
            exp_list[nf]   = e;
            ++nf;
        }

        /* compute σ(k) and A003961(k) modulo k */
        uint64_t sigma_mod = 1 % k;
        uint64_t tf_mod    = 1 % k;

        for (int i = 0; i < nf; ++i) {
            uint64_t p  = prime_list[i];
            int      e  = exp_list[i];

            /* σ contribution: product_{j=0..e} p^j modulo k */
            uint64_t sum = 1;
            uint64_t term = 1;
            for (int j = 1; j <= e; ++j) {
                term = mul_mod(term, p, k);
                sum  = add_mod(sum, term, k);
            }
            sigma_mod = mul_mod(sigma_mod, sum, k);

            /* A003961 contribution: product (next_prime(p))^e modulo k */
            uint64_t nextp = (uint64_t)next_prime_of_uint64((uint64_t)p);
            uint64_t pow = 1;
            for (int j = 0; j < e; ++j) {
                pow = mul_mod(pow, nextp, k);
            }
            tf_mod = mul_mod(tf_mod, pow, k);
        }

        seq_t g = gcd((seq_t)k, (seq_t)sigma_mod);
        g = gcd(g, (seq_t)tf_mod);

        if ((uint64_t)g > 1) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)k;
        }
    }

    return terms[n - 1];
}
