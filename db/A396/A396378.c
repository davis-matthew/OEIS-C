/*
 * A396378  Smallest m belonging to {n, n+1, n+2, ..., n+A396377(n)} that
 *          divides the product of the other integers.
 * https://oeis.org/A396378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m ∈{n,n+1,…,r} such that * m divides the product
 *            of the other numbers in the same set, * where r is the minimal
 *            integer ≥ n for which such a number exists. * * Algorithm: * –
 *            Pre‑compute the smallest prime factor table up to MAX_LIMIT
 *            (chosen * large enough for n ≤ 10000). * – For...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/*   Configuration                                                       */
/* --------------------------------------------------------------------- */
#define MAX_LIMIT       200000          /* cover all needed values          */
#define MAX_PRIME_CNT   (MAX_LIMIT/10 + 100)   /* generous upper bound            */

/* --------------------------------------------------------------------- */
/*   Global static data                                                 */
/* --------------------------------------------------------------------- */
static bool initialized = false;

static uint32_t spf[MAX_LIMIT + 1];             /* smallest prime factor          */
static uint32_t primes[MAX_PRIME_CNT];          /* list of primes                 */
static size_t   prime_cnt = 0;                  /* number of primes                */
static int32_t  prime_index[MAX_LIMIT + 1];     /* index of prime in primes[]      */

static bool    computed[MAX_LIMIT + 1] = {false};
static seq_t   cache[MAX_LIMIT + 1]        = {0};

/* --------------------------------------------------------------------- */
/*   Sieve to build SPF and prime list                                 */
/* --------------------------------------------------------------------- */
static void init_sieve(void)
{
    if (initialized) return;

    for (uint32_t i = 0; i <= MAX_LIMIT; ++i) {
        spf[i] = i;
        prime_index[i] = -1;
    }

    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == i) {                     /* i is prime */
            prime_index[i] = (int32_t)prime_cnt;
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= MAX_LIMIT) {
                for (uint32_t j = i * i; j <= MAX_LIMIT; j += i)
                    if (spf[j] == j) spf[j] = i;
            }
        }
    }
    initialized = true;
}

/* --------------------------------------------------------------------- */
/*   Factorisation helper                                               */
/* --------------------------------------------------------------------- */
static void factorise_uint32(uint32_t x, uint32_t *p_arr, uint8_t *e_arr,
                             size_t *cnt)
{
    size_t k = 0;
    while (x > 1) {
        uint32_t p = spf[x];
        uint8_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        p_arr[k] = p;
        e_arr[k] = e;
        ++k;
    }
    *cnt = k;
}

/* --------------------------------------------------------------------- */
/*   Sequence function                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_sieve();

    /* trivial case */
    if (n <= 1) return (seq_t)n;
    if (n > MAX_LIMIT) { abort(); }   /* safety for unexpected input */

    /* re‑use cached result if available */
    if (computed[(size_t)n]) return cache[(size_t)n];

    /* totals of prime exponents for the current interval */
    static uint32_t totals[MAX_PRIME_CNT];
    memset(totals, 0, prime_cnt * sizeof(uint32_t));

    /* temporary storage for factorisation results */
    uint32_t pbuf[8];
    uint8_t  ebuf[8];
    size_t   pcnt;

    uint32_t r = (uint32_t)(n - 1);   /* current right end of interval */

    while (true) {
        /* extend interval by one element */
        ++r;
        if (r > MAX_LIMIT) abort();   /* should never happen */

        factorise_uint32(r, pbuf, ebuf, &pcnt);
        for (size_t i = 0; i < pcnt; ++i) {
            int idx = prime_index[pbuf[i]];
            totals[idx] += ebuf[i];
        }

        /* test every candidate m in [n,r] */
        for (uint32_t m = (uint32_t)n; m <= r; ++m) {
            factorise_uint32(m, pbuf, ebuf, &pcnt);
            bool ok = true;
            for (size_t i = 0; i < pcnt; ++i) {
                int idx = prime_index[pbuf[i]];
                uint32_t need = 2u * ebuf[i];
                if (totals[idx] < need) { ok = false; break; }
            }
            if (ok) {
                cache[(size_t)n] = (seq_t)m;
                computed[(size_t)n] = true;
                return (seq_t)m;
            }
        }
        /* no valid m found yet – repeat with a larger r */
    }
}
