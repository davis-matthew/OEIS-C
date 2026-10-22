/*
 * A306696  Lexicographically earliest sequence of nonnegative terms such that
 *          for any n > 0 and k > 0, if a(n) >= a(n+k), then a(n+2*k) <>
 *          a(n+k).
 * https://oeis.org/A306696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A306696: lexicographically earliest sequence of nonnegative
   integers such that for any n>0 and k>0,
   if a(n) >= a(n+k) then a(n+2*k) != a(n+k). */

/* Build infrastructure shared by C and C++ builds */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(__cplusplus)                       /* C++ build ------------------------------------------------- */
#include <vector>
static std::vector<seq_t> cache_vec(1, seq_t());   /* element 0 unused */
static void grow_cache(idx_t need)
{
    if (need >= cache_vec.size())
        cache_vec.resize(need + 1);
}
static inline seq_t get_cache(idx_t idx) { return cache_vec[idx]; }
static inline void set_cache(idx_t idx, seq_t val) { cache_vec[idx] = val; }
#else                                            /* C build ----------------------------------------------------- */
static seq_t *cache_ptr = NULL;
static size_t cache_size = 0;
static void grow_cache(idx_t need)
{
    if (need < cache_size) return;
    size_t new_cap = need + 1;
    seq_t *tmp = (seq_t *)realloc(cache_ptr, new_cap * sizeof(seq_t));
    cache_ptr = tmp;
    cache_size = new_cap;
    /* new elements left uninitialized; they will be set during computation */
}
static inline seq_t get_cache(idx_t idx) { return cache_ptr[idx]; }
static inline void set_cache(idx_t idx, seq_t val) { cache_ptr[idx] = val; }
#endif

/* Markers used for locally marking forbidden values during construction */
static uint32_t *forbid_stamp = NULL;   /* array indexed by value (size_t) */
static size_t forbid_cap = 0;           /* capacity of forbid_stamp */
static uint32_t forbid_id = 1;          /* unique stamp for each position */

/* Ensure forbid_stamp has capacity at least 'cap' */
static void ensure_forbid_cap(size_t cap)
{
    if (cap <= forbid_cap) return;
    size_t new_cap = forbid_cap ? forbid_cap * 2 : 1;
    while (new_cap < cap) new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(forbid_stamp, new_cap * sizeof(uint32_t));
    if (tmp == NULL) {
        /* allocation failed – abort (our typical error policy) */
        abort();
    }
    /* zero initialise the newly allocated part */
    memset(tmp + forbid_cap, 0, (new_cap - forbid_cap) * sizeof(uint32_t));
    forbid_stamp = tmp;
    forbid_cap = new_cap;
}

static idx_t computed = 0;      /* largest index for which a value is cached */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<=0 */

    if (n > computed) {           /* compute all values up to n */
        grow_cache(n);
        for (idx_t m = computed + 1; m <= n; ++m) {
            /* Mark all values that are forbidden at position m */
            idx_t kmax = (m - 1) / 2;
            for (idx_t k = 1; k <= kmax; ++k) {
                idx_t i = m - 2 * k;
                idx_t j = m - k;
                seq_t a_i = get_cache(i);
                seq_t a_j = get_cache(j);
                if (a_i >= a_j) {
                    size_t val = (size_t)a_j;
                    ensure_forbid_cap(val + 1);
                    forbid_stamp[val] = forbid_id;
                }
            }

            /* Find the smallest non‑negative integer not forbidden */
            seq_t cand = (seq_t)0;
            while (true) {
                size_t val = (size_t)cand;
                if (val < forbid_cap && forbid_stamp[val] == forbid_id) {
                    ++cand;  /* forbidden, try next candidate */
                } else {
                    set_cache(m, cand);
                    break;  /* found allowed value */
                }
            }

            ++forbid_id;   /* move to next identifier for the next m */
        }
        computed = n;
    }
    return get_cache(n);
}
