/*
 * A375819  Numbers k such that the sum of the numbers from 1 to k and that
 *          from 1 to k+1 share the same sum of divisors.
 * https://oeis.org/A375819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = index k such that σ(T_k) = σ(T_{k+1}), where T_k =
 *            k(k+1)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the sequence on demand, caching results
   in a static array.  It factors the numbers ≤k using a small prime
   sieve (≤ 1 000 000).  It works for any seq_t/idx_t combination
   and any n ≥ 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime sieve (up to a fixed limit)                                  */
/* ------------------------------------------------------------------ */
#define PRIME_LIMIT 1000000U  /* enough for factoring numbers ≤ 1e6² */

/* Static storage for primes */
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    primes_ready = true;

    /* Initial allocation: enough for ~78k primes, fits limit. */
    primes = (uint32_t *)malloc(PRIME_LIMIT * sizeof(uint32_t));
    if (!primes) abort(); /* memory failure */

    bool *is_comp = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_comp) abort();

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

/* ------------------------------------------------------------------ */
/* Sum‑of‑divisors of an unsigned 64‑bit integer using the prime table */
/* ------------------------------------------------------------------ */
static seq_t sigma_u64(uint64_t n)
{
    seq_t result = 1;
    uint64_t temp = n;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            seq_t term_sum = 1;      /* Σ p^0 … p^e */
            uint64_t pow = 1;        /* current p^e */
            while (temp % p == 0) {
                temp /= p;
                pow *= (uint64_t)p;
                term_sum += (seq_t)pow;
            }
            result *= term_sum;
        }
    }

    if (temp > 1) {                /* remaining prime factor */
        result *= ((seq_t)1 + (seq_t)temp);
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Sequence cache                                                     */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;
static size_t seq_size = 0;        /* number of terms filled */
static size_t seq_cap   = 0;        /* allocated capacity */
static uint64_t next_k  = 1;        /* next k to test */

/* Ensure the cache has space for need terms */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1024;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* ------------------------------------------------------------------ */
/* Find the nth term (1‑based)                                          */
/* ------------------------------------------------------------------ */
static seq_t find_term(idx_t target_index)
{
    if (target_index == 0) return (seq_t)0;  /* undefined, but safe */

    init_primes();                       /* primes ready */

    while (seq_size < (size_t)target_index) {
        /* Compute σ(T_k) and σ(T_{k+1}) using multiplicativity */
        uint64_t k  = next_k;
        uint64_t k1 = k + 1;

        /* Decompose k(k+1)/2 as product of coprime integers */
        uint64_t a_k, b_k, a_k1, b_k1;

        if ((k & 1ULL) == 0) {         /* k even: k/2 and k+1 */
            a_k = k >> 1;              /* k/2 */
            b_k = k1;                  /* k+1 */
        } else {                       /* k odd: k and (k+1)/2 */
            a_k = k;
            b_k = k1 >> 1;             /* (k+1)/2 */
        }

        if ((k1 & 1ULL) == 0) {        /* k+1 even */
            a_k1 = k1 >> 1;            /* (k+1)/2 */
            b_k1 = k + 2;              /* k+2 (since k+1 +1) */
        } else {                       /* k+1 odd */
            a_k1 = k1;
            b_k1 = (k + 2) >> 1;       /* (k+2)/2 */
        }

        seq_t sigma_k  = sigma_u64(a_k) * sigma_u64(b_k);
        seq_t sigma_k1 = sigma_u64(a_k1) * sigma_u64(b_k1);

        if (sigma_k == sigma_k1) {        /* found a term */
            ensure_seq_cap(seq_size + 1);
            seq_cache[seq_size++] = (seq_t)k;
        }
        ++next_k;
    }

    return seq_cache[target_index - 1];
}

/* ------------------------------------------------------------------ */
/* Public function a(idx_t n)                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    return find_term(n);
}
