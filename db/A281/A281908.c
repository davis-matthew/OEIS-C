/*
 * A281908  a(n) = (Sum_{k=1..n} C(n,k) mod k) mod n.
 * https://oeis.org/A281908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{k=1..n} C(n,k) mod k) mod n Computation uses
 *            prime‐factor tables up to 10000. The function is memoised via
 *            static tables and works for any n≤10000 in well under 2 s
 * Range:     u8 n <= 263, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define LIMIT 10000
#define MAX_FACTORS 6   /* maximum distinct prime factors in any number ≤ LIMIT */

/* ---------- global tables ---------- */
static int primes[1229];                    /* primes up to LIMIT – 1229 is the exact count */
static int primes_cnt = 0;
static int spf[LIMIT + 1];                  /* smallest prime factor for each number */
static int prime_to_index[LIMIT + 1];       /* for quick lookup of prime index */

static uint16_t *exp_table;                 /* factorial exponent table (n, prime_index) → exponent */

/* factorisation of every number 1..LIMIT: 
   cnt  – number of distinct prime factors
   pi   – prime indices
   exp  – corresponding exponents   */
static uint8_t  factor_cnt[LIMIT + 1];
static uint16_t factor_pi[LIMIT + 1][MAX_FACTORS];
static uint8_t  factor_exp[LIMIT + 1][MAX_FACTORS];

static int initialized = 0;

/* ---------- helpers ---------- */

/* sieve to fill spf, primes list and prime_to_index */
static void init_sieve(void)
{
    int i, j;
    for (i = 0; i <= LIMIT; ++i) spf[i] = i;
    for (i = 2; i * i <= LIMIT; ++i) {
        if (spf[i] == i) {
            for (j = i * i; j <= LIMIT; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    for (i = 2; i <= LIMIT; ++i) {
        if (spf[i] == i) {           /* i is prime */
            prime_to_index[i] = primes_cnt;
            primes[primes_cnt++] = i;
        } else {
            prime_to_index[i] = 0;    /* used only for primes */
        }
    }
}

/* factorise n and store into factor_pi / factor_exp / factor_cnt */
static void factorise(int n)
{
    int cnt = 0;
    while (n > 1) {
        int p = spf[n];
        int exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        factor_pi[cnt][1] = 0;          /* unused placeholder */
        factor_pi[n][0] = prime_to_index[p];
        factor_exp[n][0] = exp;
        ++cnt;
    }
    factor_cnt[1] = 0;                  /* 1 has no prime factors */
}

/* initialise all tables lazily */
static void init_all(void)
{
    int i, j, p, exp, idx;
    uint16_t *ptr;

    init_sieve();

    /* allocate factorial exponent table */
    exp_table = (uint16_t *)malloc((LIMIT + 1) * primes_cnt * sizeof(uint16_t));
    if (!exp_table) exit(1);

    /* initialise n=0 row to zeros */
    for (j = 0; j < primes_cnt; ++j)
        exp_table[0 * primes_cnt + j] = 0;

    /* build exponent table row by row */
    for (i = 1; i <= LIMIT; ++i) {
        /* copy previous row */
        memcpy(&exp_table[i * primes_cnt], &exp_table[(i - 1) * primes_cnt], primes_cnt * sizeof(uint16_t));

        /* factorise i and add exponents */
        int x = i;
        while (x > 1) {
            p = spf[x];
            exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            idx = prime_to_index[p];
            exp_table[i * primes_cnt + idx] += (uint16_t)exp;
        }
    }

    /* factorise all numbers 1..LIMIT */
    factor_cnt[1] = 0;   /* 1 has no prime factors */
    for (i = 2; i <= LIMIT; ++i) {
        int x = i;
        int cnt = 0;
        while (x > 1) {
            p = spf[x];
            exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            factor_pi[i][cnt] = prime_to_index[p];
            factor_exp[i][cnt] = (uint8_t)exp;
            ++cnt;
        }
        factor_cnt[i] = (uint8_t)cnt;
    }

    initialized = 1;
}

/* fast modular power (uint64_t base, uint16_t exp, uint64_t mod) */
static uint64_t powmod_u64(uint64_t base, uint16_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* ---------- the required function ---------- */
seq_t a(idx_t n)
{
    if (!initialized) init_all();

    if (n == 0) return (seq_t)0;   /* undefined offset, but return 0 safely */

    uint64_t sum = 0;
    int i, j;
    for (int k = 1; k <= n; ++k) {
        uint64_t mod = (uint64_t)k;

        /* check whether C(n,k) is divisible by k */
        int cntk = factor_cnt[k];
        int p_idx, k_exp;
        int e;
        int divisible = 1;
        for (j = 0; j < cntk; ++j) {
            p_idx = factor_pi[k][j];
            k_exp = factor_exp[k][j];
            e = exp_table[n * primes_cnt + p_idx]
                - exp_table[k * primes_cnt + p_idx]
                - exp_table[(n - k) * primes_cnt + p_idx];
            if (e < k_exp) { divisible = 0; break; }
        }

        uint64_t residue;
        if (divisible) {
            residue = 0;
        } else {
            /* compute residue as product of prime powers modulo k */
            residue = 1 % mod;
            for (i = 0; i < primes_cnt; ++i) {
                e = exp_table[n * primes_cnt + i]
                    - exp_table[k * primes_cnt + i]
                    - exp_table[(n - k) * primes_cnt + i];
                if (e == 0) continue;
                uint64_t term = powmod_u64((uint64_t)primes[i] % mod, (uint16_t)e, mod);
                residue = (residue * term) % mod;
            }
        }

        sum += residue;
    }

    seq_t result = (seq_t)(sum % n);
    return result;
}
