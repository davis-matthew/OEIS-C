/*
 * A300242  Filter sequence combining gcd(n,sigma(n)) and gcd(n,phi(n)),
 *          (A009194 and A009195).
 * https://oeis.org/A300242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 8802, u32 n <= 8802, u64 n <= 8802,
 *            u128 n <= 8802, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Filter sequence combining gcd(n,σ(n)) and gcd(n,φ(n)).  The
   Cantor pairing P(a,b)=a*(a+1)/2 + b.  The sequence is the
   restricted‑growth transform of the list of Cantor values. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* include the supplied gcd routine */
#include "../utils/gcd.h"

/* C++ specific types for caching ------------------------------------------------- */
#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

/* ------------------------------------------------------------------------------- */
/* Helper: σ(n) – sum of positive divisors of n ---------------------------------- */
static seq_t sigma_of(idx_t n)
{
    seq_t sigma = (seq_t)1;
    idx_t temp  = n;
    for (idx_t d = 2; d <= temp / d; ++d) {
        if (temp % d == 0) {
            int e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }
            seq_t p = (seq_t)d;
            seq_t pow = (seq_t)1;
            for (int i = 0; i < e + 1; ++i) pow *= p;           /* p^(e+1) */
            seq_t term = (pow - (seq_t)1) / (p - (seq_t)1);
            sigma *= term;
        }
    }
    if (temp > 1) { /* last prime factor */
        seq_t p = (seq_t)temp;
        seq_t term = (p * p - (seq_t)1) / (p - (seq_t)1); /* 1 + p */
        sigma *= term;
    }
    return sigma;
}

/* Helper: φ(n) – Euler's totient of n ------------------------------------------ */
static seq_t phi_of(idx_t n)
{
    seq_t phi = (seq_t)n;
    idx_t temp = n;
    for (idx_t d = 2; d <= temp / d; ++d) {
        if (temp % d == 0) {
            while (temp % d == 0) {
                temp /= d;
            }
            seq_t p = (seq_t)d;
            phi = phi / p * (p - (seq_t)1);
        }
    }
    if (temp > 1) {
        seq_t p = (seq_t)temp;
        phi = phi / p * (p - (seq_t)1);
    }
    return phi;
}

/* ------------------------------------------------------------------------------- */
/* Cache: a(n) values ----------------------------------------------------------- */
#ifdef __cplusplus
/* C++: use std::vector which handles constructor/destructor of big integers */
static std::vector<seq_t> a_cache;
static std::vector<seq_t> value_cache;

static seq_t find_rank(seq_t val)
{
    auto it = std::find(value_cache.begin(), value_cache.end(), val);
    if (it != value_cache.end())
        return (seq_t)(it - value_cache.begin() + 1);
    value_cache.push_back(val);
    return (seq_t)(value_cache.size());
}

static void ensure_a_cache(idx_t n)
{
    while (a_cache.size() < n) {
        idx_t m = a_cache.size() + 1;
        seq_t sigma_val = sigma_of(m);
        seq_t phi_val   = phi_of(m);
        seq_t g1 = gcd((seq_t)m, sigma_val);
        seq_t g2 = gcd((seq_t)m, phi_val);

        seq_t sum    = g1 + g2;
        seq_t cantor = sum * (sum + (seq_t)1) / (seq_t)2 + g2;
        seq_t rank   = find_rank(cantor);

        a_cache.push_back(rank);
    }
}

#else /* C dynamic arrays ----------------------------------------------------- */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static size_t a_cache_len = 0;

static seq_t *value_cache = NULL;
static size_t value_cache_cap = 0;
static size_t value_cache_len = 0;

static seq_t find_rank(seq_t val)
{
    for (size_t i = 0; i < value_cache_len; ++i) {
        if (value_cache[i] == val)
            return (seq_t)(i + 1);
    }
    if (value_cache_len == value_cache_cap) {
        size_t new_cap = value_cache_cap ? value_cache_cap * 2 : 16;
        seq_t *tmp = realloc(value_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        value_cache = tmp;
        value_cache_cap = new_cap;
    }
    value_cache[value_cache_len++] = val;
    return (seq_t)(value_cache_len);
}

static void ensure_a_cache(idx_t n)
{
    while (a_cache_len < n) {
        idx_t m = a_cache_len + 1;
        seq_t sigma_val = sigma_of(m);
        seq_t phi_val   = phi_of(m);
        seq_t g1 = gcd((seq_t)m, sigma_val);
        seq_t g2 = gcd((seq_t)m, phi_val);

        seq_t sum    = g1 + g2;
        seq_t cantor = sum * (sum + (seq_t)1) / (seq_t)2 + g2;
        seq_t rank   = find_rank(cantor);

        if (a_cache_len == a_cache_cap) {
            size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
            seq_t *tmp = realloc(a_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            a_cache = tmp;
            a_cache_cap = new_cap;
        }
        a_cache[a_cache_len++] = rank;
    }
}
#endif

/* ------------------------------------------------------------------------------- */
/* Public interface -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined offset, but safe */
    ensure_a_cache(n);
#if defined(__cplusplus)
    return a_cache[n - 1];
#else
    return a_cache[n - 1];
#endif
}
