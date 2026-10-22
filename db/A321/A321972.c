/*
 * A321972  Numbers k for which A048198(k)=3.
 * https://oeis.org/A321972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k where k is the n-th integer such that exactly three of
 *            10*k-3, 10*k-1, 10*k+1, 10*k+3 are prime. (This is OEIS sequence
 *            A321972.)
 * Range:     u8 n <= 7, u16 n <= 1094, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache_vec;
static idx_t next_k = 1;                     /* next k to test */

static void ensure_cache(size_t need) {
    if (cache_vec.size() >= need) return;
    cache_vec.reserve(need);
}

static void append_cache(seq_t val) { cache_vec.push_back(val); }
static seq_t get_cache(size_t i) { return cache_vec[i]; }
static size_t cache_size() { return cache_vec.size(); }
#else
#include <stdlib.h>
static seq_t *cache = NULL;                   /* dynamic array of seq_t */
static size_t cache_len = 0, cache_cap = 0;
static idx_t next_k = 1;                     /* next k to test */

static void ensure_cache(size_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

static void append_cache(seq_t val) {
    ensure_cache(cache_len + 1);
    cache[cache_len++] = val;
}
static seq_t get_cache(size_t i) { return cache[i]; }
static size_t cache_size() { return cache_len; }
#endif

seq_t a(idx_t n) {
    if (n < 1) return 0;
    while (cache_size() < n) {
        /* Skip k divisible by 3 – one of the four numbers would be
           divisible by 3, so the condition cannot hold. */
        while (next_k % 3 == 0) next_k++;
        idx_t k = next_k++;
        seq_t c0 = (seq_t)10 * (seq_t)k - (seq_t)3;
        seq_t c1 = (seq_t)10 * (seq_t)k - (seq_t)1;
        seq_t c2 = (seq_t)10 * (seq_t)k + (seq_t)1;
        seq_t c3 = (seq_t)10 * (seq_t)k + (seq_t)3;
        int primes = 0;
        if (isPrimeGeneric(c0)) primes++;
        if (isPrimeGeneric(c1)) primes++;
        if (isPrimeGeneric(c2)) primes++;
        if (isPrimeGeneric(c3)) primes++;
        if (primes == 3) append_cache((seq_t)k);
    }
    return get_cache(n - 1);
}
