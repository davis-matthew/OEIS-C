/*
 * A241212  Primes p with an even number of digits such that each digit in the
 *          second half of p is larger by 1 than the corresponding digit in
 *          the first half.
 * https://oeis.org/A241212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth prime p whose decimal expansion has an even length
 *            2*m and each digit in the second half equals the corresponding
 *            digit in the first half plus one
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 1731, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/pow10.h"

/* cache of generated terms */
static seq_t *cache = NULL;      /* array of primes */
static size_t cache_used = 0;    /* number of primes stored */
static size_t cache_cap  = 0;    /* capacity of cache */
static size_t last_m_gen = 0;    /* largest m that has been generated */

/* grow cache if needed */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* forward declarations */
static void generate_m(size_t m, seq_t pow10_m);
static void dfs_generate(size_t pos, seq_t cur, size_t m,
                         uint8_t *digits, seq_t pow10_m);

/* ensure that at least 'need' primes are in cache */
static void ensure_nterms(idx_t need)
{
    while ((idx_t)cache_used < need) {
        /* generate next m (half‑length) */
        last_m_gen++;
        seq_t pow10_m = pow10((idx_t)last_m_gen);
        generate_m(last_m_gen, pow10_m);
    }
}

/* generate all primes for given m and append to cache */
static void generate_m(size_t m, seq_t pow10_m)
{
    uint8_t digits[32];
    dfs_generate(0, (seq_t)0, m, digits, pow10_m);
}

/* recursive DFS over the first half digits */
static void dfs_generate(size_t pos, seq_t cur, size_t m,
                         uint8_t *digits, seq_t pow10_m)
{
    if (pos == m) {
        /* build second half from digits */
        seq_t second_half = 0;
        for (size_t i = 0; i < m; ++i) {
            second_half = second_half * 10 + ((seq_t)digits[i] + 1);
        }
        seq_t full = cur * pow10_m + second_half;
        if (isPrimeGeneric(full)) {
            grow_cache(cache_used + 1);
            cache[cache_used++] = full;
        }
        return;
    }

    /* choose digit for this position (leading digit cannot be 0) */
    uint8_t start = (pos == 0) ? 1 : 0;
    for (uint8_t d = start; d <= 8; ++d) {
        digits[pos] = d;
        dfs_generate(pos + 1, cur * 10 + d, m, digits, pow10_m);
    }
}

/* required entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined below offset, but harmless */

    ensure_nterms(n);
    return cache[n - 1];
}
