/*
 * A355741  Number of ways to choose a sequence of prime factors, one of each
 *          prime index of n.
 * https://oeis.org/A355741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e||n} (ω(index_of(p)))^e where index_of(p) is the
 *            1‑based index of prime p in the ordered list of primes, ω(m) is
 *            the number of distinct prime factors of integer m. This gives
 *            the number of ways to choose a prime factor for each prime index
 *            of n (allowing repetition according to the e...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#undef idx_t
#undef seq_t
/* The types idx_t and seq_t are supplied by the build environment. */

/* Pre‑computation limits */
#define LIMIT 1000000   /* enough for n up to about 1e6 */

/* Static storage for primes and auxiliary data */
static int   *primes      = NULL;       /* array of primes up to LIMIT */
static int    prime_cnt   = 0;          /* number of primes found */
static int   *prime_index = NULL;       /* prime_index[p] = index of prime p (1‑based) */
static uint8_t *omega_val = NULL;       /* omega_val[m] = number of distinct prime factors of m */

/* Helper to build primes, prime_index, and omega_val */
static void init_precomp(void)
{
    /* Allocate arrays */
    int   *pi_arr = (int*)malloc(sizeof(int)*(LIMIT+1));
    bool  *is_com = (bool*)malloc(sizeof(bool)*(LIMIT+1));
    memset(is_com, 0, sizeof(bool)*(LIMIT+1));
    memset(pi_arr, -1, sizeof(int)*(LIMIT+1));

    int *pr = (int*)malloc(sizeof(int)*(LIMIT+1)); /* enough space for primes */
    int cnt = 0;

    /* Simple sieve */
    for (int i = 2; i <= LIMIT; ++i) {
        if (!is_com[i]) {
            pi_arr[i] = ++cnt;           /* 1‑based index */
            pr[cnt-1] = i;
            if ((long long)i * i <= LIMIT) {
                for (long long j = (long long)i * i; j <= LIMIT; j += i)
                    is_com[(int)j] = true;
            }
        }
    }

    prime_cnt  = cnt;
    primes     = pr;
    prime_index= pi_arr;

    /* Compute omega values for indices 1..prime_cnt using sieve of distinct primes */
    omega_val = (uint8_t*)calloc(prime_cnt+1, sizeof(uint8_t));
    for (int p = 2; p <= prime_cnt; ++p) {
        if (omega_val[p] == 0) { /* p is prime */
            for (int j = p; j <= prime_cnt; j += p)
                ++omega_val[j];
        }
    }

    free(is_com);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Lazy initialization */
    static bool initialized = false;
    if (!initialized) {
        init_precomp();
        initialized = true;
    }

    if (n == 1) return 1;

    seq_t result = 1;
    idx_t tmp = n;

    for (size_t i = 0; i < (size_t)prime_cnt && (idx_t)primes[i] * primes[i] <= tmp; ++i) {
        int p = primes[i];
        if (tmp % p == 0) {
            int exp = 0;
            do {
                tmp /= p;
                ++exp;
            } while (tmp % p == 0);

            int idx = prime_index[p];          /* index of prime p */
            int w   = omega_val[idx];          /* Ω(idx) */
            /* multiply w^exp */
            for (int k = 0; k < exp; ++k)
                result *= (seq_t)w;
        }
    }

    if (tmp > 1) { /* tmp is a prime > sqrt(original n) */
        int p = (int)tmp;
        int idx = prime_index[p];
        int w = omega_val[idx];
        result *= (seq_t)w;
    }

    return result;
}
