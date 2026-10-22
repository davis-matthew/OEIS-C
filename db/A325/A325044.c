/*
 * A325044  Heinz numbers of integer partitions whose sum of parts is greater
 *          than or equal to their product.
 * https://oeis.org/A325044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth Heinz number of an integer partition whose sum of
 *            parts is greater than or equal to their product
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A Heinz number of a partition (y1,…,yk) is the product
    prime(y1) * … * prime(yk) (prime(n) is the nth prime: 2,3,5,…).
    For a number y >= 2, let e be the exponent of the prime 2 in y,
    and let val = y / 2^e (the odd part).  If val == 1 the
    partition can be all 1's and the condition holds.

    If val > 1, factor val into primes p^cnt.
    For each prime p let idx = the index of p in the prime sequence.
    Let
        P = product over all p of idx^cnt
        S = sum over all p of idx^cnt
    Then y satisfies the condition iff
        P <= S + e.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*  seq_t and idx_t are defined by the build.  Do NOT redefine them.  */

/* --- Sieve for smallest prime factors and prime indices --- */
static idx_t *spf = NULL;         /* smallest prime factor of each integer */
static uint32_t *prime_idx = NULL; /* index of prime (1‑based), 0 if composite */
static idx_t sieve_limit = 0;      /* current size of the sieve */

/* Allocate and build sieve up to `limit`.  */
static void build_sieve(idx_t limit)
{
    if (sieve_limit == limit) return;
    if (spf) { free(spf); free(prime_idx); }
    spf = (idx_t*)calloc((size_t)limit + 1, sizeof(idx_t));
    prime_idx = (uint32_t*)calloc((size_t)limit + 1, sizeof(uint32_t));
    if (!spf || !prime_idx) exit(1);          /* out of memory */

    idx_t *primes = (idx_t*)malloc((size_t)limit * sizeof(idx_t));
    idx_t primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {                       /* i is prime */
            spf[i] = i;
            prime_idx[i] = ++primes_cnt;
            primes[primes_cnt - 1] = i;
        }
        for (idx_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            idx_t v = p * i;
            if (v > limit) break;
            spf[v] = p;
            if (p == spf[i]) break;
        }
    }
    free(primes);
    sieve_limit = limit;
}

/* Ensure we have a sieve covering at least `n`.  */
static void ensure_sieve(idx_t n)
{
    if (n <= sieve_limit) return;
    idx_t new_limit = sieve_limit ? sieve_limit * 2 : 2;
    while (new_limit < n) new_limit <<= 1;
    build_sieve(new_limit);
}

/* Test whether a number y satisfies the Heinz‑partition condition.  */
static bool is_heinz(seq_t y)
{
    if (y < 2) return false;          /* sequence starts at 2 */

    /* separate factor 2 */
    uint64_t e = 0;
    uint64_t temp = (uint64_t)y;      /* y fits in 64 bits; seq_t may be larger but factors stay <= y */
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e;
    }
    uint64_t val = temp;               /* odd part */

    if (val == 1) {                    /* y is a power of 2 */
        return e >= 1;
    }

    /* factor val using spf */
    __uint128_t P = 1;                 /* product of indices */
    uint64_t S = 0;                    /* sum of indices */

    while (val > 1) {
        uint32_t p = (uint32_t)(spf[val]);   /* p is prime factor */
        uint64_t cnt = 0;
        while (val % p == 0) {
            val /= p;
            ++cnt;
        }
        uint32_t idx = prime_idx[p];
        for (uint64_t i = 0; i < cnt; ++i) {
            P *= idx;
            S += idx;
        }
    }
    __uint128_t rhs = (__uint128_t)(S) + e;
    return P <= rhs;
}

/* --- Sequence cache --- */
static seq_t *cache = NULL;       /* array of sequence values */
static idx_t cache_len = 0;       /* number of values cached */
static idx_t cache_cap = 0;       /* allocated capacity */

/* Generate sequence values until we have at least `n` entries. */
static void generate_upto(idx_t n)
{
    static uint64_t next_y = 2;    /* next integer to test */

    if (!cache) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    if (!cache) exit(1);

    while (cache_len < n) {
        seq_t val = (seq_t)next_y;
        ensure_sieve(next_y);
        if (is_heinz(val)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *new_arr = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!new_arr) exit(1);
                cache = new_arr;
            }
            cache[cache_len++] = val;
        }
        ++next_y;
    }
}

/* --- Public function required by the build system --- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but return 0 safely */
    generate_upto(n);
    return cache[n - 1];
}
