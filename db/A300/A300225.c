/*
 * A300225  Filter sequence combining A296078(n) and A296091(n), the prime
 *          signatures of phi(n)+1 and sigma(n)-1, with a(1) = 1.
 * https://oeis.org/A300225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth of Cantor pair( A296078(n), A296091(n)
 *            ), where A296078(n)=prime signature of φ(n)+1 and
 *            A296091(n)=prime signature of σ(n)-1. a(1)=1
 * Range:     u8 n <= 2393, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9286 (2026-11-03)
 * Helpers:   factor_u64, mem, num_digits_idx, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/num_digits_idx.h"
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Prime signature: encode the sorted multiset of exponents of prime factorisation. */
static seq_t prime_signature(uint64_t v)
{
    if (v == 0) return 0;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(v, pr, ex, &cnt);
    if (cnt == 0) return 0;

    /* sort exponents ascending (small array, insertion sort). */
    for (size_t i = 1; i < cnt; ++i) {
        uint64_t key = ex[i];
        size_t j = i;
        while (j > 0 && ex[j - 1] > key) {
            ex[j] = ex[j - 1];
            --j;
        }
        ex[j] = key;
    }

    seq_t enc = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = ex[i];
        int d = num_digits_idx(e);
        seq_t pow10_d = pow10((idx_t)d);
        enc = enc * pow10_d + e;
    }
    return enc;
}

/* Reallocate or grow an array of elements of size elem_size to accommodate needed elements. */
#ifdef __cplusplus
template <class T>
static T *resize_array(T *ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        ptr = mem_resize(T, ptr, (new_cap * elem_size) / sizeof(T));
        *cap = new_cap;
    }
    return ptr;
}
#else
static void *resize_array(void *ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        ptr = realloc(ptr, new_cap * elem_size);
        *cap = new_cap;
    }
    return ptr;
}
#endif

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Cache of previously computed a(n) values. */
    static idx_t max_n = 0;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Map from Cantor pair values to their restricted‑growth rank. */
    static seq_t *map_keys = NULL;
    static seq_t *map_vals = NULL;
    static size_t map_len = 0;
    static size_t map_cap_keys = 0;
    static size_t map_cap_vals = 0;

    if (n == 0) return 0; /* Undefined, but safe guard. */

    /* Grow cache if needed. */
    if (n > max_n) {
        cache = (seq_t *)resize_array(cache, &cache_cap, (size_t)(n + 1), sizeof(seq_t));
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* Compute A296078(i) = prime signature of φ(i)+1. */
            seq_t phi_val = totient_seq((seq_t)i) + 1;
            seq_t sig_phi = prime_signature((uint64_t)phi_val);

            /* Compute A296091(i) = prime signature of σ(i)-1. */
            seq_t sigma_val = sigma_of((idx_t)i);
            seq_t sigma_minus_one = sigma_val - 1;
            seq_t sig_sigma = prime_signature((uint64_t)sigma_minus_one);

            /* Cantor pairing function P(a,b). */
            seq_t sum = sig_phi + sig_sigma;
            seq_t pair = sum * (sum + 1) / 2 + sig_sigma;

            /* Find or insert this pair into the map. */
            size_t pos;
            for (pos = 0; pos < map_len; ++pos) {
                if (map_keys[pos] == pair) break;
            }
            seq_t rank_val;
            if (pos < map_len) {
                rank_val = map_vals[pos];
            } else {
                rank_val = (seq_t)(map_len + 1);

                map_keys = (seq_t *)resize_array(map_keys, &map_cap_keys,
                                                map_len + 1, sizeof(seq_t));
                map_vals = (seq_t *)resize_array(map_vals, &map_cap_vals,
                                                map_len + 1, sizeof(seq_t));
                map_keys[map_len] = pair;
                map_vals[map_len] = rank_val;
                ++map_len;
            }
            cache[i] = rank_val;
        }
        max_n = n;
    }
    return cache[n];
}
