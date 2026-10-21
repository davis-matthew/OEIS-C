/*
 * A242867  Discriminant of the pure cubic field Q(m^(1/3)), where m =
 *          A004709(n) is the n-th cubefree number.
 * https://oeis.org/A242867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Discriminant of the pure cubic field Q(m^(1/3)) where m is the
 *            nth cubefree integer (A004709). a(n) = 1 if m = 1 a(n) = -3 *
 *            h^2 * k^2 if m ≡ ±1 (mod 9) a(n) = -27 * h^2 * k^2 otherwise
 *            where m = h * k^2, h is squarefree, k is sqrt of largest square
 *            dividing m. For cubefree m, k is the product of...
 * Range:     i8 n <= 2, i16 n <= 32, i32 n <= 7420, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache for cubefree numbers */
static uint64_t *cubefree_cache = NULL;
static size_t cubefree_count = 0;
static size_t cubefree_cap   = 0;
static uint64_t current_candidate = 1;

/* static cache for smallest prime factor (spf) array */
static uint32_t *spf = NULL;
static size_t spf_size = 0;

/* enlarge small‑prime‑factor array up to at least LIMIT */
static void ensure_spf(size_t limit)
{
    if (limit <= spf_size) return;
    size_t new_size = limit + 1;
    uint32_t *new_spf = (uint32_t *)realloc(spf, new_size * sizeof(uint32_t));
    if (!new_spf) return;            /* out of memory – silently fail */
    spf = new_spf;
    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;
    if (limit >= 1) spf[1] = 1;
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_size = limit;
}

/* test if X is cubefree using the current spf array */
static bool is_cubefree(uint64_t X)
{
    while (X > 1) {
        uint32_t p = spf[X];
        int cnt = 0;
        while (X % p == 0) {
            X /= p;
            ++cnt;
            if (cnt >= 3) return false;
        }
    }
    return true;
}

/* enlarge the cubefree cache to hold at least N values */
static void grow_cubefree(size_t needed)
{
    if (needed <= cubefree_cap) return;
    size_t new_cap = cubefree_cap ? cubefree_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(cubefree_cache, new_cap * sizeof(uint64_t));
    if (!tmp) return;                /* out of memory – silently fail */
    cubefree_cache = tmp;
    cubefree_cap = new_cap;
}

/* ensure we have the first N cubefree numbers cached */
static void ensure_cubefree_n(idx_t N)
{
    if (cubefree_count >= (size_t)N) return;
    while (cubefree_count < (size_t)N) {
        uint64_t x = current_candidate;
        ensure_spf((size_t)x);
        if (is_cubefree(x)) {
            grow_cubefree(cubefree_count + 1);
            cubefree_cache[cubefree_count++] = x;
        }
        ++current_candidate;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, but guard anyway */
    ensure_cubefree_n(n);
    uint64_t m = cubefree_cache[n - 1];
    if (m == 1) return (seq_t)1;

    /* factor m: compute h (squarefree part) and k (product of primes with exponent 2) */
    uint64_t tmp = m;
    seq_t h = 1, k = 1;
    while (tmp > 1) {
        uint32_t p = spf[(size_t)tmp];
        int cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        if (cnt == 1) h *= (seq_t)p;
        else if (cnt == 2) k *= (seq_t)p;
    }

    seq_t term = h * h * (k * k);
    uint64_t mod9 = m % 9;
    if (mod9 == 1 || mod9 == 8)
        return - (seq_t)3 * term;
    else
        return - (seq_t)27 * term;
}
