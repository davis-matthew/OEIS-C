/*
 * A327933  Numbers such that the smallest prime factor of their arithmetic
 *          derivative is 3.
 * https://oeis.org/A327933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 8963, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers such that the smallest prime factor of their arithmetic derivative is 3.
   The function builds the sequence incrementally, searching successive integers
   until the required index is reached.  It uses prime factorisation to test the
   condition that the arithmetic derivative is an odd multiple of 3. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Prime sieve (sufficient for the requested range) ---------- */
#define PRIME_LIMIT 1000000U          /* enough for n up to ~10^12 */
static uint32_t *prime_list = NULL;  /* array of primes up to PRIME_LIMIT */
static size_t   prime_cnt   = 0;     /* number of primes stored */

static void init_primes(void)
{
    if (prime_list != NULL)
        return;                      /* already initialised */

    /* simple Sieve of Eratosthenes */
    prime_list = (uint32_t *)malloc((PRIME_LIMIT + 1) * sizeof(uint32_t));
    uint8_t *sieve = (uint8_t *)malloc((PRIME_LIMIT + 1) * sizeof(uint8_t));
    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        sieve[i] = 0;

    prime_cnt = 0;
    for (uint32_t p = 2; p <= PRIME_LIMIT; ++p) {
        if (!sieve[p]) {
            prime_list[prime_cnt++] = p;
            if ((uint64_t)p * (uint64_t)p <= PRIME_LIMIT) {
                for (uint32_t j = p * p; j <= PRIME_LIMIT; j += p)
                    sieve[j] = 1;
            }
        }
    }
    free(sieve);
}

/* ---------- Arithmetic derivative modulo 2 and 3 ---------- */
/* n' = n * Σ_{p^e||n} e/p   ->  n' = Σ e * (n/p)  */
static void derivative_mod23(uint64_t n, int *mod2, int *mod3)
{
    uint64_t tmp = n;
    int sum2 = 0;
    int sum3 = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > tmp)
            break;
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            uint64_t term = n / p;
            int t2 = term & 1;            /* term mod 2 */
            int t3 = term % 3;            /* term mod 3 */
            int e2 = e & 1;               /* e mod 2 */
            int e3 = e % 3;               /* e mod 3 */

            sum2 = (sum2 + (t2 & e2)) & 1;          /* modulo 2 */
            sum3 = (sum3 + (t3 * e3)) % 3;          /* modulo 3 */
        }
    }

    if (tmp > 1) { /* remaining prime factor */
        uint64_t term = n / tmp;
        int t2 = term & 1;
        int t3 = term % 3;
        sum2 = (sum2 + t2) & 1;
        sum3 = (sum3 + t3) % 3;
    }

    *mod2 = sum2;
    *mod3 = sum3;
}

/* ---------- Sequence storage ---------- */
static seq_t *seq_cache = NULL;   /* dynamic array of sequence terms   */
static size_t seq_cap   = 0;      /* allocated capacity                */
static size_t seq_len   = 0;      /* number of terms currently stored  */
static idx_t  last_checked = 1;   /* last n tested during incremental scan */

static void append_term(seq_t val)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 128 : seq_cap * 2;
        /* allocate new array and copy existing terms */
#if defined(__cplusplus)
        seq_t *new_arr = new seq_t[new_cap];
        for (size_t i = 0; i < seq_len; ++i)
            new_arr[i] = seq_cache[i];
        delete[] seq_cache;
#else
        seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
#endif
        seq_cache = new_arr;
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1 */

    if (seq_len < n) {                          /* need more terms */
        init_primes();                          /* ensure primes are ready */

        while (seq_len < n) {
            ++last_checked;
            uint64_t cur = last_checked;

            int mod2, mod3;
            derivative_mod23(cur, &mod2, &mod3);

            /* derivative odd (mod2==1) and divisible by 3 (mod3==0) */
            if (mod2 == 1 && mod3 == 0)
                append_term((seq_t)cur);
        }
    }
    return seq_cache[n - 1]; /* zero‑based index in cache */
}
