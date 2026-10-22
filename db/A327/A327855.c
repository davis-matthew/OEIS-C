/*
 * A327855  Irregular triangle read by rows: T(n,k) = [x^k]
 *          (Sum_{i=0..prime(n)-1} (1+Legendre(i,prime(n))) * x^i)^2, for 0 <=
 *          k <= 2*prime(n)-2.
 * https://oeis.org/A327855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   OEIS A327855 – Irregular triangle read by rows
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by generating each row of the triangle once and caching the
   entries sequentially.  For the n-th prime p the row is
   convolution of the vector A[i] = 1 + Legendre(i,p)
   (A[0] = 1, A[residue] = 2, all others 0).  The convolution is
   done by enumerating the non‑zero indices (0 and all quadratic
   residues modulo p).  Complexity is O(p²) for each row, which is
   negligible for the primes required up to n ≈ 10000. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --- global cache of flattened triangle entries --------------------- */
static seq_t *cache = NULL;       /* flattened rows */
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* --- prime list ----------------------------------------------------- */
static uint32_t *prime_list = NULL;          /* primes in order of appearance */
static size_t primes_len = 0;
static size_t primes_cap = 0;
static uint32_t last_prime = 0;

/* ensure cache has space for at least 'needed' elements */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* ensure prime list has space for at least 'needed' primes */
static void ensure_prime_capacity(size_t needed)
{
    if (needed <= primes_cap) return;
    size_t new_cap = primes_cap ? (primes_cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    if (!tmp) exit(EXIT_FAILURE);
    prime_list = tmp;
    primes_cap = new_cap;
}

/* return the next prime > last_prime and add it to prime_list */
static uint32_t next_prime(void)
{
    uint32_t cand = last_prime + 1;
    if (cand <= 2) cand = 2;
    if (cand > 2 && (cand & 1) == 0) cand++;   /* skip even numbers */

    for (;;) {
        bool prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) {
            ensure_prime_capacity(primes_len + 1);
            prime_list[primes_len++] = cand;
            last_prime = cand;
            return cand;
        }
        cand += 2;   /* next odd candidate */
        if (cand == 3) ;    /* no special case needed */
    }
}

/* compute the next row of the triangle and append its entries to the cache */
static void compute_next_row(void)
{
    uint32_t p = next_prime();              /* n-th prime */
    size_t p_sz = (size_t)p;
    size_t row_len = 2 * p_sz - 1;          /* length of the row */

    /* Boolean array marking quadratic residues modulo p */
    unsigned char *is_res = (unsigned char *)calloc(p_sz, sizeof(unsigned char));
    if (!is_res) exit(EXIT_FAILURE);
    for (uint32_t x = 1; x < p; ++x) {
        uint32_t r = (uint32_t)((uint64_t)x * x % p);
        is_res[r] = 1;
    }

    /* Collect residue indices (excluding 0) into a compact array */
    size_t r_cnt = 0;
    for (uint32_t i = 1; i < p; ++i) if (is_res[i]) ++r_cnt;
    uint32_t *res = (uint32_t *)malloc(r_cnt * sizeof(uint32_t));
    if (!res) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (uint32_t i = 1; i < p; ++i) if (is_res[i]) res[idx++] = i;
    free(is_res);

    /* Prepare cache space for this row */
    ensure_cache_capacity(cache_len + row_len);
    seq_t *row_vals = &cache[cache_len];
    for (size_t k = 0; k < row_len; ++k) row_vals[k] = 0;

    /* (0,0) pair contributes 1 */
    row_vals[0] = 1;

    /* Pairs involving 0 */
    for (size_t i = 0; i < r_cnt; ++i) {
        uint32_t r0 = res[i];
        row_vals[r0] += (seq_t)4;          /* (0,r0) + (r0,0) */
    }

    /* Pairs among residues */
    for (size_t i = 0; i < r_cnt; ++i) {
        uint32_t r0 = res[i];

        /* Self pair (r0, r0) */
        size_t kss = (size_t)(2 * r0);
        row_vals[kss] += (seq_t)4;

        /* Pairs with larger residues */
        for (size_t j = i + 1; j < r_cnt; ++j) {
            uint32_t r1 = res[j];
            size_t k = (size_t)(r0 + r1);
            row_vals[k] += (seq_t)8;      /* (r0,r1) + (r1,r0) */
        }
    }

    free(res);
    cache_len += row_len;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; a(1) is the first element */
    size_t target = (size_t)(n - 1);  /* 0‑based index in flattened triangle */

    /* Generate rows until we have at least target+1 entries */
    while (cache_len <= target) {
        compute_next_row();
    }
    return cache[target];
}
