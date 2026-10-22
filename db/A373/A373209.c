/*
 * A373209  Numbers k such that k^2 - 1 and k^2 + 1 have 8 divisors each.
 * https://oeis.org/A373209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that tau(k^2-1) = tau(k^2+1) = 8
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Each term can be computed by checking k sequentially, using fast
   factorisation of k-1, k+1 (k^2-1) via precomputed smallest
   prime factors, and trial division up to sqrt(k^2+1) for k^2+1.
   The results are memoised in a static array so that calls with
   arbitrary indices are fast. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Configurable limits                                                */
/* --------------------------------------------------------------------- */
#define MAX_LIMIT 1000000     /* Sieve limit for small factorizations */

/* --------------------------------------------------------------------- */
/*  Global data for primes and SPF                                     */
/* --------------------------------------------------------------------- */
static uint32_t primes[80000];
static size_t primes_cnt = 0;
static uint32_t *spf = NULL;

/* --------------------------------------------------------------------- */
/*  Prime sieve (linear)                                                */
/* --------------------------------------------------------------------- */
static void init_primes_and_spf(void)
{
    size_t n = MAX_LIMIT;
    spf = (uint32_t*)malloc((n + 1) * sizeof(uint32_t));
    if (!spf) return;               /* allocation failure – should not happen */

    spf[0] = 0;
    spf[1] = 1;
    for (size_t i = 2; i <= n; ++i) spf[i] = 0;

    for (size_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = (uint32_t)i;
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i] || i * (size_t)p > n) break;
            spf[i * (size_t)p] = p;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Divisor count for numbers <= MAX_LIMIT (using precomputed SPF)      */
/* --------------------------------------------------------------------- */
static uint64_t tau_small(uint64_t n)
{
    if (n <= 1) return 1;
    uint64_t res = 1;
    while (n > 1) {
        uint32_t p = spf[(size_t)n];
        int exp = 0;
        do {
            n /= p;
            ++exp;
        } while (n % p == 0);
        res *= (uint64_t)(exp + 1);
    }
    return res;
}

/* --------------------------------------------------------------------- */
/*  Divisor count for arbitrary 64‑bit integers (trial division)       */
/*  The routine stops early if the divisor count exceeds 8.             */
/* --------------------------------------------------------------------- */
static uint64_t tau_big(uint64_t n)
{
    uint64_t m = n;
    uint64_t tau = 1;
    int distinct = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > m) break;
        if (m % p == 0) {
            ++distinct;
            int exp = 0;
            do {
                m /= p;
                ++exp;
            } while (m % p == 0);
            tau *= (uint64_t)(exp + 1);
            if (tau > 8 || distinct > 3) return tau;   /* early abort */
        }
    }
    if (m > 1) {          /* remaining prime factor */
        ++distinct;
        tau *= 2;
    }
    return tau;
}

/* --------------------------------------------------------------------- */
/*  The required sequence function                                      */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Lazy initialisation */
    static bool init = false;
    static idx_t next_k = 2;            /* next candidate to test               */
    static seq_t *buf = NULL;           /* dynamic array of found terms          */
    static size_t buf_size = 0;         /* number of terms stored                */
    static size_t buf_cap  = 0;         /* allocated capacity                    */

    if (!init) {
        init = true;
        init_primes_and_spf();

        buf_cap = 32;
        buf = (seq_t*)malloc(buf_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least `n` of them */
    while (buf_size < (size_t)n) {
        uint64_t k = (uint64_t)next_k;

        uint64_t tau1;
        if ((k & 1ULL) == 0) {   /* k even */
            tau1 = tau_small(k - 1) * tau_small(k + 1);
        } else {                 /* k odd */
            uint64_t a_part = (k - 1) / 2;
            uint64_t b_part = (k + 1) / 2;
            tau1 = 3ULL * tau_small(a_part) * tau_small(b_part); /* tau(4)=3 */
        }

        if (tau1 == 8) {
            /* k^2 + 1 fits comfortably into 64 bits for expected k */
            unsigned __int128 kk = (unsigned __int128)k * k + 1;
            uint64_t k2p1 = (uint64_t)kk;
            uint64_t tau2 = tau_big(k2p1);

            if (tau2 == 8) {
                /* store the term */
                if (buf_size == buf_cap) {
                    buf_cap *= 2;
                    buf = (seq_t*)realloc(buf, buf_cap * sizeof(seq_t));
                }
                buf[buf_size++] = (seq_t)k;
            }
        }

        ++next_k;
    }

    return buf[(size_t)n - 1];
}
