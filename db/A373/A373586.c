/*
 * A373586  Numbers k for which A373149(k) is even, where A373149 is fully
 *          additive with a(2) = 1 and a(p) = prevprime(p) for odd primes p.
 * https://oeis.org/A373586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k>0 such that the parity of the total number
 *            of prime factors of k/3^v₃(k) is even (equivalently,
 *            Ω(k/3^v₃(k)) mod 2 == 0)
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by a simple linear search up to the nth
   qualifying number.  A small sieve of size 2 000 000 is built once
   to factor numbers efficiently via smallest‑prime‑factor tables. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- global tables --- */
#define SPF_LIMIT     2000000U          /* sieve size */

static uint32_t *spf          = NULL;  /* smallest prime factor table */
static uint32_t *prime_list   = NULL;  /* list of primes <= SPF_LIMIT */
static size_t    prime_cnt    = 0;      /* number of primes stored */
static bool      sieve_ready  = false; /* flag */

/* build sieve and populate spf and prime_list */
static void build_sieve(void)
{
    if (sieve_ready)
        return;

    /* allocate tables */
    spf        = (uint32_t*)calloc((size_t)SPF_LIMIT + 1, sizeof(uint32_t));
    prime_list = (uint32_t*)calloc(200000, sizeof(uint32_t)); /* enough for primes <= 2e6 */
    if (!spf || !prime_list) return; /* allocation failed */

    for (uint32_t i = 2; i <= SPF_LIMIT; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i]        = i;
            prime_list[prime_cnt++] = i;

            if ((uint64_t)i * i <= SPF_LIMIT) {
                for (uint32_t j = i * i; j <= SPF_LIMIT; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }
    sieve_ready = true;
}

/* check whether k satisfies the parity condition */
static bool good_k(uint64_t k)
{
    /* remove all factors of 3 */
    while (k % 3ULL == 0)
        k /= 3ULL;

    int parity = 0;

    /* factorization using spf table for small numbers */
    if (k <= SPF_LIMIT && spf[k] != 0) {
        while (k > 1) {
            uint32_t p   = spf[(size_t)k];
            int      cnt = 0;
            while (k % p == 0) {
                k /= p;
                ++cnt;
            }
            parity ^= (cnt & 1);
        }
    } else { /* trial division using precomputed primes */
        uint64_t temp = k;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * (uint64_t)p > temp)
                break;
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            parity ^= (cnt & 1);
        }
        if (temp > 1)            /* remaining prime factor */
            parity ^= 1;
    }
    return parity == 0;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n=0, return 0 */

    build_sieve();

    uint64_t found = 0;
    uint64_t k     = 1;

    while (found < n) {
        if (good_k(k))
            ++found;
        if (found == n)
            break;
        ++k;
    }

    return (seq_t)k;
}
