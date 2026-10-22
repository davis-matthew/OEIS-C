/*
 * A318838  Product_{d|n, A007431(d) > 0} prime(A007431(d)), where A007431 is
 *          the Möbius transform of Euler's totient function.
 * https://oeis.org/A318838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{d·d|n , A007431(d)>0} prime( A007431(d) ) where
 *            A007431(m) = Σ_{t|m} μ(t)·φ(m/t)
 * Range:     u8 n <= 20, u16 n <= 62, u32 n <= 191, u64 n <= 671,
 *            u128 n <= 2015, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* -------------------------------------------------------------------------- */
/*  static data                                                                */
static idx_t      cached_max   = 0;              /* highest n pre‑computed   */
static idx_t     *phi          = NULL;           /* φ(0…cached_max)           */
static int8_t    *mu           = NULL;           /* μ(0…cached_max)           */
static int64_t   *a007         = NULL;           /* A007431(0…cached_max)     */

static uint32_t  *prime_list   = NULL;           /* list of primes (1‑based)  */
static size_t     prime_cnt    = 0;              /* number of primes stored   */

/* -------------------------------------------------------------------------- */
/*  build a list of primes up to a fixed limit (used for prime(k))           */
static void ensure_primes(void)
{
    if (prime_list != NULL)
        return;                                 /* already done             */

    /* limit to generate ~20000th prime (~224737) – enough for n ≤ 10000   */
    const idx_t LIMIT = 250000;
    bool *sieve = (bool*)calloc((size_t)(LIMIT + 1), sizeof(bool));
    if (!sieve) exit(1);
    for (idx_t i = 2; i <= LIMIT; ++i)
        sieve[i] = true;

    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = false;

    /* collect primes                                                     */
    prime_cnt = 0;
    size_t cap = 2000;
    prime_list = (uint32_t*)malloc(cap * sizeof(uint32_t));
    if (!prime_list) exit(1);

    for (idx_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) {
            if (prime_cnt >= cap) {
                cap *= 2;
                prime_list = (uint32_t*)realloc(prime_list, cap * sizeof(uint32_t));
                if (!prime_list) exit(1);
            }
            prime_list[prime_cnt++] = (uint32_t)i;
        }
    free(sieve);
}

/* -------------------------------------------------------------------------- */
/*  return the k‑th prime (1‑based)                                           */
static seq_t prime_at(idx_t k)
{
    if (k == 0) return 0;
    if (prime_list == NULL)
        ensure_primes();
    /* assume k ≤ prime_cnt for all inputs that will be asked for          */
    return (seq_t)prime_list[(size_t)(k - 1)];
}

/* -------------------------------------------------------------------------- */
/*  pre‑compute φ, μ and A007431 up to max_n                                 */
static void ensure_precalc(idx_t max_n)
{
    if (max_n <= cached_max)
        return;                                 /* already done             */

    size_t sz = (size_t)(max_n + 1);

    phi = (idx_t*)realloc(phi, sz * sizeof(idx_t));
    mu  = (int8_t*)realloc(mu,  sz * sizeof(int8_t));
    a007= (int64_t*)realloc(a007, sz * sizeof(int64_t));
    if (!phi || !mu || !a007) exit(1);

    phi[0] = 0;  mu[0] = 0;  a007[0] = 0;
    for (size_t i = 1; i < sz; ++i) {
        phi[i] = (idx_t)i;
        mu[i]  = 1;
        a007[i] = 0;
    }

    /* linear sieve to compute φ and μ                                 */
    size_t primes_cap = 16;
    size_t primes_sz  = 0;
    idx_t *primes = (idx_t*)malloc(primes_cap * sizeof(idx_t));
    if (!primes) exit(1);

    for (size_t i = 2; i < sz; ++i) {
        if (phi[i] == (idx_t)i) {                /* i is prime          */
            if (primes_sz >= primes_cap) {
                primes_cap *= 2;
                primes = (idx_t*)realloc(primes, primes_cap * sizeof(idx_t));
                if (!primes) exit(1);
            }
            primes[primes_sz++] = (idx_t)i;
            phi[i] = (idx_t)i - 1;
            mu[i]  = -1;
        }
        for (size_t pj = 0; pj < primes_sz; ++pj) {
            idx_t p  = primes[pj];
            idx_t ip = (idx_t)i * p;
            if (ip >= sz)
                break;
            if (i % p == 0) {
                phi[ip] = phi[i] * p;
                mu[ip]  = 0;
                break;
            } else {
                phi[ip] = phi[i] * phi[p];
                mu[ip]  = -mu[i];
            }
        }
    }
    free(primes);

    /* compute A007431: A007431[m] = Σ_{d|m} μ(d)·φ(m/d)                    */
    for (idx_t d = 1; d < sz; ++d) {
        int8_t mud = mu[d];
        if (mud == 0)
            continue;
        for (idx_t m = d; m < sz; m += d) {
            a007[m] += (int64_t)mud * (int64_t)(phi[m / d]);
        }
    }

    cached_max = max_n;
}

/* -------------------------------------------------------------------------- */
/*  sequence function                                                          */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                                 /* safety                   */

    ensure_precalc(n);
    seq_t prod = 1;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d                                                     */
            if (a007[d] > 0)
                prod *= prime_at((idx_t)a007[d]);

            /* paired divisor n/d if distinct                                 */
            idx_t d2 = n / d;
            if (d2 != d && a007[d2] > 0)
                prod *= prime_at((idx_t)a007[d2]);
        }
    }
    return prod;
}
