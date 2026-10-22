/*
 * A371179  Positive integers with fewer distinct prime factors (A001221) than
 *          distinct divisors of prime indices (A370820).
 * https://oeis.org/A371179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer such that the number of
 *            distinct prime factors of m is less than the number of distinct
 *            divisors of the indices of primes dividing m
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Static data used to factor numbers and compute prime indices         */
/* --------------------------------------------------------------------- */
#define SIEVE_LIMIT 500000U      /* upper bound for primes and indices       */

static uint32_t *spf           = NULL;   /* smallest prime factor               */
static uint32_t *prime_index   = NULL;   /* index of a prime, 0 if not prime     */
static int      *last_seen     = NULL;   /* marker array to count unique divisors*/
static unsigned int  seen_marker = 0;     /* global counter for uniqueness        */
static bool          initialized = false;

/* --------------------------------------------------------------------- */
/*  Linear sieve to fill spf, prime_index and allocate last_seen          */
/* --------------------------------------------------------------------- */
static void init_sieve(void) {
    uint32_t limit = SIEVE_LIMIT;
    spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    prime_index = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    uint32_t *primes = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    size_t primes_cnt = 0;

    for (uint32_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            primes[primes_cnt] = i;
            prime_index[i] = (uint32_t)(primes_cnt + 1); /* indices start at 1 */
            ++primes_cnt;
        }
        for (size_t j = 0; j < primes_cnt && primes[j] <= spf[i] &&
             (uint64_t)i * primes[j] <= limit; ++j) {
            uint32_t v = i * primes[j];
            spf[v] = primes[j];
        }
    }
    free(primes);
    last_seen = (int *)calloc(limit + 1, sizeof(int));
    initialized = true;
}

/* --------------------------------------------------------------------- */
/*  Main function computing a(n)                                          */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (!initialized) init_sieve();

    idx_t m = 3;            /* start from the first candidate               */
    idx_t count = 0;        /* number of terms found so far                  */

    /* loop until we find the nth term */
    while (1) {
        /* factor m using spf */
        uint32_t temp = (uint32_t)m;   /* m ≤ SIEVE_LIMIT guaranteed */
        uint32_t omega = 0;            /* distinct prime factors count */
        int indices[10];               /* distinct prime indices list (max 6 for m ≤ 5e5) */
        int idx_cnt = 0;

        while (temp > 1) {
            uint32_t p = spf[temp];
            while (temp % p == 0) temp /= p;   /* skip multiplicity */
            ++omega;
            indices[idx_cnt++] = (int)prime_index[p]; /* prime_index is non‑zero */
        }

        /* count distinct divisors of the set of indices */
        ++seen_marker;            /* new marker for this m */
        int tau = 0;              /* distinct divisor count */

        for (int ii = 0; ii < idx_cnt; ++ii) {
            int idx = indices[ii];
            for (int d = 1; d * d <= idx; ++d) {
                if (idx % d == 0) {
                    if (last_seen[d] != (int)seen_marker) {
                        last_seen[d] = (int)seen_marker;
                        ++tau;
                    }
                    int e = idx / d;
                    if (e != d && last_seen[e] != (int)seen_marker) {
                        last_seen[e] = (int)seen_marker;
                        ++tau;
                    }
                }
            }
        }

        if (omega < tau) {
            ++count;
            if (count == n) return (seq_t)m;
        }
        ++m;
    }
}
