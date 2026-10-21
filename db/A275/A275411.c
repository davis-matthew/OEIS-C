/*
 * A275411  Least k such that sigma(k*n) = sigma(k*n-1), or 0 if no such k
 *          exists.
 * https://oeis.org/A275411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least k such that σ(k*n) = σ(k*n-1), otherwise 0. If k
 *            exists it is known to be ≤ 1 000 000 for n up to 10 000
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------- */
/* Utility: small primes sieve (up to 1e5)  */
/* --------------------------------------- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_K          1000000U   /* search bound for k */
#define PRIME_LIMIT    100000U   /* sqrt(1e10) */

/* storage for primes up to PRIME_LIMIT */
static uint32_t primes[PRIME_LIMIT + 1];
static size_t prime_cnt = 0;
static unsigned char is_comp[PRIME_LIMIT + 1];

/* smallest prime factor for numbers up to MAX_K */
static uint32_t spf[MAX_K + 1];

/* one‑time initialization flag */
static bool initialized = false;

/* one‑time sieve setup */
static void init_sieve(void) {
    if (initialized) return;

    /* primes up to PRIME_LIMIT */
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }

    /* smallest prime factor for 2 … MAX_K */
    for (uint32_t i = 2; i <= MAX_K; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            if ((uint64_t)i * i <= MAX_K) {
                for (uint64_t j = (uint64_t)i * i; j <= MAX_K; j += i)
                    if (!spf[j]) spf[j] = i;
            }
        }
    }

    initialized = true;
}

/* --------------------------------------- */
/* Helper structs and functions            */
/* --------------------------------------- */
typedef struct { uint64_t p; int e; } prime_exp_t;

/* factorization of a small number x ≤ MAX_K using spf */
static void factor_small(uint32_t x, prime_exp_t fac[], int *cnt) {
    *cnt = 0;
    while (x > 1) {
        uint32_t p = spf[x];
        int e = 0;
        while (x % p == 0) { x /= p; ++e; }
        fac[(*cnt)++] = (prime_exp_t){p, e};
    }
}

/* factorization of a large number m (≤1e10) using precomputed primes */
static void factor_large(uint64_t m, prime_exp_t fac[], int *cnt) {
    *cnt = 0;
    uint64_t tmp = m;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p;
        if (pp * pp > tmp) break;
        if (tmp % pp == 0) {
            int e = 0;
            while (tmp % pp == 0) { tmp /= pp; ++e; }
            fac[(*cnt)++] = (prime_exp_t){pp, e};
        }
    }
    if (tmp > 1) fac[(*cnt)++] = (prime_exp_t){tmp, 1};
}

/* compute σ(m) from its prime factorization */
static seq_t sigma_from_factor(const prime_exp_t fac[], int cnt) {
    unsigned __int128 res = 1;
    for (int i = 0; i < cnt; ++i) {
        uint64_t p = fac[i].p;
        int e = fac[i].e;
        unsigned __int128 term = 1;
        unsigned __int128 pow = 1;
        for (int j = 0; j < e; ++j) {
            pow *= p;
            term += pow;
        }
        res *= term;
    }
    return (seq_t)res;
}

/* σ(m) for a possibly large m (≤1e10) */
static seq_t sigma_large(uint64_t m) {
    prime_exp_t fac[32];
    int cnt;
    factor_large(m, fac, &cnt);
    return sigma_from_factor(fac, cnt);
}

/* --------------------------------------- */
/* a(n) : the required OEIS sequence   */
/* --------------------------------------- */
seq_t a(idx_t n) {
    init_sieve();

    uint64_t num = (uint64_t)n;

    for (uint32_t k = 1; k <= MAX_K; ++k) {
        uint64_t m = num * (uint64_t)k;
        if (m <= 1) continue;          /* skip σ(0) undefined */

        seq_t s1 = sigma_large(m);
        seq_t s2 = sigma_large(m - 1);
        if (s1 == s2) return (seq_t)k;
    }

    return 0;   /* no k ≤ MAX_K found */
}
