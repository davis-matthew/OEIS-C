/*
 * A339908  Odd squarefree numbers k > 1 for which bigomega(phi(k)) <
 *          bigomega(k-1), where bigomega gives the number of prime divisors,
 *          counted with multiplicity.
 * https://oeis.org/A339908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1863, u32 n <= 9806, u64 n <= 9806,
 *            u128 n <= 9806, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A339908:
   a(n) = the nth odd square‑free integer k > 1 such that
   bigomega(phi(k)) < bigomega(k-1).  The implementation
   generates terms lazily and caches them, so calls may arrive
   in any order.  It uses a small pre‑computed prime table
   to factor numbers efficiently. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- static data ----------
   We build a sieve up to LIMIT and store:
   - spf[i]   : smallest prime factor of i
   - bigomega[i] : number of prime factors of i (with multiplicity)
   - primes[]   : list of primes up to LIMIT
   The LIMIT is chosen large enough for n up to ~10000.  */
#define LIMIT 2000000U

static uint32_t *spf   = NULL;      /* smallest prime factor */
static uint8_t  *bigomega = NULL;   /* bigomega array   */
static uint32_t *primes = NULL;     /* prime list       */
static size_t   prime_cnt = 0;      /* number of primes */
static bool     sieve_ready = false;

/* ---------- helper functions ---------- */

/* Build the sieve and auxiliary tables.  Called once on first use. */
static void build_sieve(void)
{
    if (sieve_ready) return;

    const size_t N = LIMIT;
    spf = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    if (!spf) return;
    for (size_t i = 0; i <= N; ++i) spf[i] = 0;

    /* sieve for smallest prime factors */
    for (size_t i = 2; i <= N; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            if (i <= N / i) {
                for (size_t j = i * i; j <= N; j += i)
                    if (!spf[j]) spf[j] = (uint32_t)i;
            }
        }
    }

    /* build prime list (size estimate: N / log N) */
    primes = (uint32_t *)malloc((N / 10) * sizeof(uint32_t));
    if (!primes) return;
    prime_cnt = 0;
    for (size_t i = 2; i <= N; ++i)
        if (spf[i] == (uint32_t)i) primes[prime_cnt++] = (uint32_t)i;

    /* bigomega array */
    bigomega = (uint8_t *)malloc((N + 1) * sizeof(uint8_t));
    if (!bigomega) return;
    bigomega[0] = bigomega[1] = 0;
    for (size_t i = 2; i <= N; ++i) {
        uint32_t p = spf[i];
        bigomega[i] = 1 + bigomega[i / p];
    }

    sieve_ready = true;
}

/* Test square‑free for n <= LIMIT using spf. */
static bool is_squarefree(uint64_t n)
{
    if (n > LIMIT) return false;              /* out of range */
    uint32_t val = (uint32_t)n;
    uint32_t last = 0;
    while (val > 1) {
        uint32_t p = spf[val];
        if (p == last) return false;          /* repeated prime factor */
        last = p;
        val /= p;
        while (val % p == 0) return false;    /* repeated factor */
    }
    return true;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* offset 1, undefined for 0 */

    static seq_t *cache  = NULL;
    static size_t sz     = 0;               /* number cached */
    static size_t cap    = 0;               /* cache capacity */
    static uint64_t next_k = 3;             /* next odd number to test */

    /* ensure sieve tables exist */
    if (!sieve_ready) build_sieve();
    if (!sieve_ready) return (seq_t)0;      /* allocation failure */

    /* grow cache if needed */
    if (cap == 0) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
    }

    /* generate terms until we have at least n */
    while (sz < (size_t)n) {
        if (next_k > LIMIT) return (seq_t)0;   /* out of pre‑computed range */

        if (is_squarefree(next_k)) {
            /* compute bigomega(phi(k)) and bigomega(k-1) */
            uint32_t kval = (uint32_t)next_k;
            uint32_t temp = kval;
            uint32_t last = 0;
            uint8_t bigomega_phi = 0;

            while (temp > 1) {
                uint32_t p = spf[temp];
                if (p == last) {
                    /* should not happen due to square‑free test */
                    temp /= p;
                    continue;
                }
                last = p;
                bigomega_phi += bigomega[p - 1]; /* p-1 < k <= LIMIT */

                /* remove all occurrences of p (none should remain) */
                while ((temp % p) == 0) temp /= p;
            }

            uint8_t bigomega_kminus = bigomega[kval - 1];  /* k-1 <= LIMIT */

            if (bigomega_phi < bigomega_kminus) {
                /* store the qualifying k */
                if (sz >= cap) {
                    size_t new_cap = cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;
                    cache = tmp;
                    cap = new_cap;
                }
                cache[sz++] = (seq_t)next_k;
            }
        }

        next_k += 2;   /* next odd integer */
    }

    return cache[n - 1];
}
