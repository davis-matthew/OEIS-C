/*
 * A275769  Least k such that n divides sigma(k!) (k > 0).
 * https://oeis.org/A275769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n divides σ(k!) (k > 0)
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- helper: modular multiplication for uint64_t ---------------- */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
#ifdef __GNUC__
    /* GCC/clang support __int128, safe for C++17 as well */
    return (unsigned __int128)a * b % mod;
#else
    /* generic but slower fallback */
    uint64_t result = 0;
    a %= mod;
    while (b)
    {
        if (b & 1ULL)
            result = (result + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return result;
#endif
}

/* ---------- prime sieve and smallest prime factor -------------------- */
#define PRIME_LIMIT 2000000UL            /* sufficient for n up to ~10⁴ */

static uint32_t *g_prime_list = NULL;   /* primes in ascending order   */
static size_t   g_prime_cnt   = 0;      /* number of primes            */
static uint32_t *g_spf_table  = NULL;   /* g_spf_table[i] = smallest prime factor of i */

static void init_primes(void)
{
    if (g_prime_list != NULL) return;   /* already built */

    uint32_t limit = PRIME_LIMIT;
    g_spf_table = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    if (!g_spf_table) exit(1);

    /* linear sieve */
    uint32_t *primes_tmp = (uint32_t *)malloc(limit / 10 * sizeof(uint32_t));
    size_t pcnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
    {
        if (g_spf_table[i] == 0)
        {
            g_spf_table[i] = i;
            if (pcnt == limit / 10)
                primes_tmp = (uint32_t *)realloc(primes_tmp, (pcnt + limit / 10) * sizeof(uint32_t));
            primes_tmp[pcnt++] = i;
        }
        for (size_t j = 0; j < pcnt; ++j)
        {
            uint32_t p = primes_tmp[j];
            uint64_t val = (uint64_t)i * p;
            if (val > limit) break;
            g_spf_table[val] = p;
            if (p == g_spf_table[i]) break;
        }
    }

    g_prime_list = (uint32_t *)malloc(pcnt * sizeof(uint32_t));
    memcpy(g_prime_list, primes_tmp, pcnt * sizeof(uint32_t));
    g_prime_cnt = pcnt;
    free(primes_tmp);
}

/* binary search: index of prime p in g_prime_list (p is guaranteed to be prime) */
static inline size_t prime_index(uint32_t p)
{
    size_t lo = 0, hi = g_prime_cnt;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        uint32_t midp = g_prime_list[mid];
        if (midp == p) return mid;
        if (midp < p)  lo = mid + 1;
        else           hi = mid;
    }
    return 0;   /* should never happen */
}

/* ------------------------------- sequence -------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1, n = 1 divides everything */

    init_primes();                        /* ensure primes & spf are ready */

    uint64_t n64 = (uint64_t)n;

    /* arrays to hold sum and power of each prime modulo n */
    uint64_t *sum_mod = (uint64_t *)calloc(g_prime_cnt, sizeof(uint64_t));
    uint64_t *pow_mod = (uint64_t *)calloc(g_prime_cnt, sizeof(uint64_t));
    if (!sum_mod || !pow_mod) exit(1);

    for (size_t i = 0; i < g_prime_cnt; ++i)
    {
        sum_mod[i] = 1;
        pow_mod[i] = 1;
    }

    for (idx_t k = 1; ; ++k)
    {
        uint32_t t = (uint32_t)k;
        while (t > 1)
        {
            uint32_t p = g_spf_table[t];
            uint32_t cnt = 0;
            while (t % p == 0)
            {
                t /= p;
                ++cnt;
            }
            size_t idx = prime_index(p);
            for (uint32_t j = 0; j < cnt; ++j)
            {
                pow_mod[idx] = (pow_mod[idx] * (uint64_t)p) % n64;
                sum_mod[idx] = (sum_mod[idx] + pow_mod[idx]) % n64;
            }
        }

        /* compute σ(k!) modulo n */
        uint64_t prod = 1;
        for (size_t i = 0; i < g_prime_cnt; ++i)
        {
            if (g_prime_list[i] > (uint32_t)k) break;
            prod = mul_mod64(prod, sum_mod[i], n64);
            if (prod == 0) break;           /* early exit if divisible */
        }

        if (prod == 0)          /* n divides σ(k!) */
        {
            free(sum_mod);
            free(pow_mod);
            return (seq_t)k;
        }
    }
}
