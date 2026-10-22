/*
 * A400523  The exponent of the highest power of 2 dividing the n-th number
 *          whose prime factorization exponents are squares (A197680).
 * https://oeis.org/A400523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = exponent of the highest power of 2 dividing the n‑th *
 *            number whose prime factorisation exponents are perfect squares *
 *            (OEIS A400523). The implementation generates the numbers in *
 *            ascending order by scanning integers, testing whether all prime
 *            * exponents are squares, and keeps a cache o...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static size_t cache_len = 0;       /* number of terms stored      */
static size_t cache_cap = 0;       /* allocated capacity of cache */
static seq_t *cache = NULL;        /* cache of exponent values    */

static size_t spf_limit = 0;       /* upper bound of SPF array     */
static uint32_t *spf = NULL;       /* smallest prime factor array  */

static bool sq_exp[65] = {false}; /* square‑exponent lookup       */
static bool sq_inited = false;     /* init flag                    */

/* ----------------------------------------------------------------------------
 * initialise the square exponent lookup table
 * ---------------------------------------------------------------------------*/
static void init_square_lookup(void)
{
    if (sq_inited) return;
    for (int i = 0; i < 65; ++i) sq_exp[i] = false;
    for (int i = 0; i * i < 65; ++i) sq_exp[i * i] = true;
    sq_inited = true;
}

/* ----------------------------------------------------------------------------
 * dynamic extension of SPF array up to LIMIT (inclusive).
 * IMPLEMENTED as a fresh sieve each time the limit grows.
 * ---------------------------------------------------------------------------*/
static void ensure_spf_limit(size_t limit)
{
    if (limit <= spf_limit) return;

    size_t new_limit = (spf_limit == 0) ? 2048 : spf_limit;
    while (new_limit < limit) new_limit <<= 1;   /* double until big enough */

    uint32_t *new_spf = (uint32_t *)calloc(new_limit + 1, sizeof(uint32_t));
    for (size_t i = 2; i <= new_limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (i * i <= new_limit) {
                for (size_t j = i * i; j <= new_limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }
    free(spf);
    spf = new_spf;
    spf_limit = new_limit;
}

/* ----------------------------------------------------------------------------
 * factorise m and verify all prime exponents are perfect squares.
 * It assumes SPF array is large enough for m (ensure_spf_limit called).
 * ---------------------------------------------------------------------------*/
static bool factor_check_squares(uint64_t m)
{
    while (m > 1) {
        uint32_t p = spf[m];
        uint8_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (!sq_exp[exp]) return false;
    }
    return true;
}

/* ----------------------------------------------------------------------------
 * allocate / grow the cache array as needed
 * ---------------------------------------------------------------------------*/
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* ----------------------------------------------------------------------------
 * generate terms until we have at least n entries in cache
 * ---------------------------------------------------------------------------*/
static void generate_until(idx_t n)
{
    static uint64_t last_checked = 0;     /* last integer examined      */
    if (n <= cache_len) return;           /* already have enough        */

    while (cache_len < n) {
        ++last_checked;
        /* first, count factors of 2 */
        uint8_t exp2 = 0;
        uint64_t tmp = last_checked;
        while ((tmp & 1ULL) == 0) {
            ++exp2;
            tmp >>= 1;
        }
        if (!sq_exp[exp2]) continue;      /* 2‑exponent not a square   */

        /* Ensure SPF array covers this number (including odd part) */
        ensure_spf_limit(last_checked);

        /* check remaining prime exponents */
        if (factor_check_squares(tmp)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)exp2;
        }
    }
}

/* ----------------------------------------------------------------------------
 * Public entry function
 * ---------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1 */
    init_square_lookup();
    if ((idx_t)cache_len < n) generate_until(n);
    return cache[(size_t)(n - 1)];
}
