/*
 * A331185  a(n) = n - prime(A067004(n)), where A067004 is the ordinal
 *          transform of number of divisors of n (A000005).
 * https://oeis.org/A331185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = n - A000040(A067004(n))
 * Range:     i8 n <= 143, i16 n <= 8506, i32 n <= 8506, i64 n <= 8506,
 *            i128 n <= 8506, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A331185  -
 *
 *  A067004(n) is the ordinal transform of the divisor function a(n)=d(n).
 *  For each n, let d(n) be the number of divisors of n.
 *  A067004(n) is 1 plus the number of previous occurrences of the same divisor
 *  count among 1 ≤ k < n.  Thus A067004(n) is the ordinal rank of d(n) in the
 *  prefix 1..n.
 *
 *  The nth prime is taken using the standard 1‑based indexing of A000040.
 *  The function below pre‑computes all values for n ≤ 10000 on the first call
 *  and returns the stored result for subsequent calls.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Maximum n that may be requested.  The problem guarantees n ≤ 10000. */
#define NMAX 10000

static seq_t  a_cache[NMAX + 1];          /* a(n) values */
static int    tau_cache[NMAX + 1];        /* divisor counts d(n)   */
static int    t_cache[NMAX + 1];          /* ordinal transform t(n) */
static int*   prime_list;                /* first NMAX primes, 1‑based */
static bool   initialized = false;

/*------------------------------------------------------------*/
/*  Compute all necessary tables on first use.                */
/*------------------------------------------------------------*/
static void init(void)
{
    /* ----------  smallest prime factor sieve  ------------- */
    static uint32_t spf[NMAX + 1];
    for (idx_t i = 0; i <= NMAX; ++i) spf[i] = (uint32_t)i;
    for (idx_t i = 2; i * i <= NMAX; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (idx_t j = i * i; j <= NMAX; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }

    /* ----------  divisor function d(n)  ------------------ */
    tau_cache[1] = 1;
    int max_tau = 1;
    for (idx_t n = 2; n <= NMAX; ++n) {
        idx_t x = n;
        int  d  = 1;
        while (x > 1) {
            int p = spf[x];
            int e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            d *= (e + 1);
        }
        tau_cache[n] = d;
        if (d > max_tau) max_tau = d;
    }

    /* ----------  ordinal transform t(n)  ----------------- */
    int *freq = (int*)calloc(max_tau + 1, sizeof(int));
    for (idx_t n = 1; n <= NMAX; ++n) {
        int tau = tau_cache[n];
        int t   = ++freq[tau];
        t_cache[n] = t;
    }
    free(freq);

    /* ----------  prime list (first NMAX primes)  --------- */
    const int prime_limit = 200000;           /* enough for >NMAX primes */
    bool *is_prime = (bool*)malloc((prime_limit + 1) * sizeof(bool));
    for (int i = 0; i <= prime_limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= prime_limit; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= prime_limit; j += i)
                is_prime[j] = false;

    prime_list = (int*)malloc((NMAX + 1) * sizeof(int));
    int cnt = 0;
    for (int i = 2; i <= prime_limit && cnt < NMAX; ++i)
        if (is_prime[i]) {
            ++cnt;
            prime_list[cnt] = i;               /* 1‑based index */
        }
    free(is_prime);

    /* ----------  a(n) values  -------------------------- */
    for (idx_t n = 1; n <= NMAX; ++n) {
        int t  = t_cache[n];
        int p  = prime_list[t];                /* p = t‑th prime (1‑based) */
        a_cache[n] = (seq_t)n - (seq_t)p;
    }
}

/*------------------------------------------------------------*/
/*  External function as required by the exerciser.          */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (!initialized) {
        init();
        initialized = true;
    }
    /* The exercise guarantees n ≥ 1 and n ≤ NMAX.  No bounds checking
       beyond this is performed to keep the routine fast.        */
    return a_cache[(size_t)n];
}
