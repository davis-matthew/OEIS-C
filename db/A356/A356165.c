/*
 * A356165  a(n) = n minus the smallest positive k such that n divides
 *          k*A003961(k), where A003961 is fully multiplicative with a(p) =
 *          nextprime(p).
 * https://oeis.org/A356165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 268, u16 n <= 9604, u32 n <= 9604, u64 n <= 9604,
 *            u128 n <= 9604, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = n minus the smallest k>0 such that n divides k*A003961(k),
   where A003961 is fully multiplicative and A003961(p) = nextprime(p) for prime p.
   The function returns a(n). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*--- Constants & global tables -----------------------------------------*/
#define MAX_LIMIT 40000   /* sufficient for n up to ~10 000 */

static uint32_t spf[MAX_LIMIT + 1];   /* smallest prime factor */
static uint32_t primes[MAX_LIMIT + 1];
static size_t   prime_cnt = 0;
static uint32_t nextprime_of_prime[MAX_LIMIT + 1];
static bool     init_done  = false;

/*--- Helper to compute integer power modulo using 128‑bit arithmetic ----*/
static seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod)
{
    if (mod == 1) return 0;
    seq_t res = 1 % mod;
    seq_t b   = base % mod;
    while (exp) {
        if (exp & 1)
            res = (unsigned __int128)res * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1;
    }
    return res;
}

/*--- Initialisation of primes, SPF and next‑prime table ----------------*/
static void init(void)
{
    if (init_done) return;
    init_done = true;

    /* SPF sieve */
    for (uint32_t i = 0; i <= MAX_LIMIT; ++i) spf[i] = i;
    spf[0] = 0;  spf[1] = 1;
    for (uint32_t i = 2; i * i <= MAX_LIMIT; ++i) {
        if (spf[i] == i) {                     /* i is prime */
            for (uint32_t j = i * i; j <= MAX_LIMIT; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    /* List of primes and next‑prime mapping */
    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) if (spf[i] == i)
        primes[prime_cnt++] = i;

    for (size_t i = 0; i + 1 < prime_cnt; ++i)
        nextprime_of_prime[primes[i]] = primes[i + 1];
    nextprime_of_prime[primes[prime_cnt - 1]] = 0;   /* last prime */
}

/*--- Main sequence function --------------------------------------------*/
seq_t a(idx_t n)
{
    init();                            /* one‑time initialisation */

    if (n == 0) return (seq_t)0;       /* safety for non‑offset calls */

    seq_t mod = (seq_t)n;              /* local copy to avoid repeated casts */

    /* Brute‑force search for the minimal k (always exists with k = n) */
    for (uint64_t k = 1; k <= n; ++k) {
        seq_t g_mod = (mod == 1) ? 0 : 1 % mod;   /* compute G(k) mod n */
        uint64_t temp = k;
        while (temp > 1) {
            uint32_t p = spf[(uint32_t)temp];
            uint64_t e = 0;
            while (temp % p == 0) { temp /= p; ++e; }

            uint64_t base_full = (uint64_t)p * (uint64_t)nextprime_of_prime[p];
            seq_t   base_mod  = (seq_t)(base_full % mod);
            seq_t   pow_mod   = mod_pow(base_mod, e, mod);
            g_mod = (unsigned __int128)g_mod * pow_mod % mod;
            if (g_mod == 0) break;            /* early exit if already 0 */
        }

        if (g_mod == 0)                      /* n | G(k) */
            return (seq_t)(n - k);
    }

    /* Fallback (should never be reached; k = n is always a solution) */
    return (seq_t)0;
}
