/*
 * A355058  Numbers m such that d(m) mod 6 = 3, where d(m) is the number of
 *          divisors of m.
 * https://oeis.org/A355058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 234, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A355058: numbers m such that d(m) mod 6 = 3.
   d(m) is the divisor function. All terms are squares: m = x²
   where x has at least one prime exponent ≡ 1 (mod 3). */

#include <stdlib.h>
#include <stdbool.h>

/* ---- Smallest prime factor table ---- */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Build spf table up to `limit`. The table entries are of type idx_t. */
static void build_spf(idx_t limit)
{
    spf = (idx_t *)realloc(spf, (limit + 1) * sizeof(idx_t));
    if (!spf) return;            /* allocation failure – unlikely in tests */
    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    spf_limit = limit;
}

/* Ensure the spf table covers at least `need`. */
static void ensure_spf(idx_t need)
{
    if (!spf || need > spf_limit) {
        idx_t new_limit = spf_limit ? spf_limit << 1 : 256;
        while (new_limit < need) new_limit <<= 1;
        build_spf(new_limit);
    }
}

/* ---- Dynamic array for memoisation ---- */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t newcap = (*cap) ? (*cap) * 2U : 16U;
    if (newcap < needed) newcap = needed;
    *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *cap = newcap;
}

/* ---- Sequence generation ---- */
extern seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Next integer whose square we examine */
    static idx_t next_root = 2U;

    /* Lazy initialization of spf table */
    if (!spf) build_spf(256U);

    /* Generate terms until we have at least `n` entries */
    while (cache_len < n) {
        ensure_spf(next_root);

        idx_t x = next_root;
        bool good = false;
        while (x > 1U) {
            idx_t p = spf[x];
            idx_t exp = 0U;
            while (x % p == 0U) {
                x /= p;
                ++exp;
            }
            if (exp % 3U == 1U) {
                good = true;
                break;
            }
        }

        if (good) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)next_root * (seq_t)next_root;
        }

        ++next_root;
    }

    return cache[n - 1];
}
