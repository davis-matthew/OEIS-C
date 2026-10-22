/*
 * A400443  Numbers k such that k is an automorphic number in at least one
 *          base b in the range [2, k-2].
 * https://oeis.org/A400443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   max_digit_base2, pow_leq_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A400443.
   The sequence contains all integers k for which there exists a base
   b in the range [2, k-2] such that k is automorphic in base b. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/max_digit_base2.h"
#include "../utils/pow_leq_u128.h"

/* ---------- integer utilities ---------- */

/* Return base^exp as unsigned __int128. */
static __uint128_t pow_u128(uint64_t base, unsigned int exp)
{
    __uint128_t result = 1;
    for (unsigned int i = 0; i < exp; ++i) result *= base;
    return result;
}

/* Floor integer kth root of n: largest x such that x^k <= n. */
static uint64_t kth_root_floor(uint64_t n, unsigned int k)
{
    if (k == 1) return n;
    uint64_t low = 1, high = n;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (pow_leq_u128(mid, k, n)) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* ---------- check if a number is automorphic in some base ---------- */

/* Returns true if k has a base b ∈ [2, k-2] such that k is automorphic. */
static bool has_automorphic_base(uint64_t k)
{
    if (k <= 2) return false;          /* no valid base in [2, k-2] */

    __uint128_t prod = (__uint128_t)k * (k - 1);   /* k(k-1) */

    unsigned int max_d = max_digit_base2(k);

    for (unsigned int d = 2; d <= max_d; ++d) {
        /* Determine the valid range of b for this digit count d. */
        uint64_t b_start = kth_root_floor(k, d) + 1;          /* k < b^d  → b > k^(1/d) */
        uint64_t b_max_candidate = kth_root_floor(k, d - 1);  /* k >= b^(d-1) → b <= k^(1/(d-1)) */
        uint64_t b_end = b_max_candidate;
        if (b_end > k - 2) b_end = k - 2;
        if (b_start < 2) b_start = 2;
        if (b_start > b_end) continue;

        for (uint64_t b = b_start; b <= b_end; ++b) {
            /* Compute b^d with overflow guard. */
            __uint128_t b_pow = 1;
            bool overflow = false;
            for (unsigned int i = 0; i < d; ++i) {
                if (b_pow > prod / b) { overflow = true; break; }
                b_pow *= b;
            }
            if (overflow || b_pow > prod) break;  /* further b will only increase b_pow */
            if (prod % b_pow == 0) return true;
        }
    }
    return false;
}

/* ---------- sequence storage and retrieval ---------- */

static seq_t *seq_cache = NULL;     /* array of discovered terms */
static size_t cache_len = 0;        /* number of stored terms */
static size_t cache_cap = 0;        /* capacity of seq_cache */
static uint64_t next_candidate = 1; /* next integer to test for being in the sequence */

/* Ensure the cache can hold at least `need` elements. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();        /* allocation failure */
    seq_cache = new_arr;
    cache_cap = new_cap;
}

/* Generate terms until cache_len reaches n. */
static void generate_terms_up_to(idx_t n)
{
    while (cache_len < n) {
        /* search for the next valid k */
        while (!has_automorphic_base(next_candidate)) ++next_candidate;
        ensure_cache_capacity(cache_len + 1);
        seq_cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* ---------- public interface ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, but guard for 0 */
    generate_terms_up_to(n);
    return seq_cache[n - 1];
}
