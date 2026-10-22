/*
 * A361289  For the odd numbers 2n + 1, the least practical number r such that
 *          2n + 1 = r + p where p is prime.
 * https://oeis.org/A361289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least practical r such that 2n+1 = r + p, p prime
 * Range:     u8 n <= 3859, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_practical_uint64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition as given in OEIS A361289. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/is_practical_uint64.h"

static seq_t *cache = NULL;          /* cache[0] holds a(1), etc. */
static size_t cache_cap = 0;
static size_t cache_len = 0;         /* maximum n computed + 1 */

static seq_t *practicals = NULL;    /* list of practical numbers */
static size_t practical_cap = 0;
static size_t practical_len = 0;

/* ---------- primality test for 64‑bit values ----------
   Use the provided is_prime(idx_t).  */
static inline bool is_prime_uint64(uint64_t x) {
    return is_prime((idx_t)x);
}

/* ---------- grow practicals capacity ----------
   Ensures the array can hold at least 'need' elements. */
static void grow_practical_cap(size_t need) {
    if (need <= practical_cap)
        return;
    size_t new_cap = practical_cap * 2 + 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_ptr = (seq_t *)realloc(practicals, new_cap * sizeof(seq_t));
    practicals = new_ptr;
    practical_cap = new_cap;
}

/* ---------- extend practicals up to 'limit' (inclusive) ----------
   Practical numbers are generated lazily on demand. */
static void extend_practicals_to(uint64_t limit) {
    if (practical_len == 0) {
        grow_practical_cap(1);
        practicals[0] = (seq_t)1;  /* first practical number */
        practical_len = 1;
    }
    uint64_t candidate = (uint64_t)practicals[practical_len - 1] + 1;
    while (candidate <= limit) {
        if (is_practical_uint64(candidate)) {
            grow_practical_cap(practical_len + 1);
            practicals[practical_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
}

/* ---------- grow cache capacity ----------
   Ensures the cache can store values up to index 'need' (1‑based). */
static void grow_cache_cap(size_t need) {
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap * 2 + 32;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* ---------- main OEIS sequence function ----------
   Returns a(n) for n >= 1, caching results for later calls. */
seq_t a(idx_t n) {
    if (n < 1)
        return 0;   /* undefined for n < 1 */

    /* compute values up to n if needed */
    if ((size_t)n > cache_len) {
        grow_cache_cap((size_t)n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            uint64_t odd = 2u * (uint64_t)i + 1u;          /* 2n+1 */
            /* we need practical numbers up to (odd-2) */
            uint64_t limit = (odd > 2) ? odd - 2 : 0;
            extend_practicals_to(limit);

            seq_t best = 0;
            for (size_t j = 0; j < practical_len; ++j) {
                uint64_t r = (uint64_t)practicals[j];
                if (r > odd - 2)
                    break;
                uint64_t p = odd - r;
                if (is_prime_uint64(p)) {
                    best = (seq_t)r;
                    break;
                }
            }
            cache[i - 1] = best;
        }
        cache_len = n;
    }
    return cache[n - 1];
}
