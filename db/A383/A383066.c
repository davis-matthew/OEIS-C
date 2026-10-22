/*
 * A383066  A 2-regular sequence associated with the number of divisors of n^2
 *          + 1 (A193432).
 * https://oeis.org/A383066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 148, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) obeys the recurrences
 *   a(1) = 0, a(2) = 1, a(3) = 1
 *   a(4*n)   = 2*a(2*n)   - a(n)
 *   a(4*n+1) = 2*a(2*n)   + a(2*n+1)
 *   a(4*n+2) = 2*a(2*n+1) + a(2*n)
 *   a(4*n+3) = 2*a(2*n+1) - a(n)
 *
 * The implementation uses recursive memoisation.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <unordered_map>
#endif

/* Forward declaration of the sequence function */
seq_t a(idx_t n);

#ifdef __cplusplus  /* ---------- C++ implementation ---------- */

static std::unordered_map<idx_t, seq_t> cache;

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 1;

    auto it = cache.find(n);
    if (it != cache.end())
        return it->second;

    seq_t result;
    switch (n & 3u)  /* n % 4 */
    {
        case 0: { /* n = 4*k */
            idx_t k = n >> 2;
            seq_t a_k   = a(k);
            seq_t a_2k  = a(k << 1);
            result = 2 * a_2k - a_k;
            break;
        }
        case 1: { /* n = 4*k + 1 */
            idx_t k = (n - 1) >> 2;
            seq_t a_k   = a(k);
            seq_t a_2k  = a(k << 1);
            seq_t a_2k1 = a((k << 1) + 1);
            result = 2 * a_2k + a_2k1;
            break;
        }
        case 2: { /* n = 4*k + 2 */
            idx_t k = (n - 2) >> 2;
            seq_t a_k   = a(k);
            seq_t a_2k  = a(k << 1);
            seq_t a_2k1 = a((k << 1) + 1);
            result = 2 * a_2k1 + a_2k;
            break;
        }
        case 3: { /* n = 4*k + 3 */
            idx_t k = (n - 3) >> 2;
            seq_t a_k   = a(k);
            seq_t a_2k  = a(k << 1);
            seq_t a_2k1 = a((k << 1) + 1);
            result = 2 * a_2k1 - a_k;
            break;
        }
    }

    cache[n] = result;
    return result;
}

#else  /* ---------- C implementation ---------- */

static seq_t *cache = NULL;
static unsigned char *vis = NULL;
static size_t cache_cap = 0;

static void ensure_cache(idx_t max_n)
{
    if ((size_t)(max_n + 1) <= cache_cap)
        return;

    size_t new_cap = (size_t)(max_n + 1);
    new_cap <<= 1;        /* double the capacity */
    if (new_cap < 8)
        new_cap = 8;

    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    unsigned char *new_vis = (unsigned char*)realloc(vis, new_cap);
    if (!new_cache || !new_vis)
        abort();   /* allocation failed */

    /* initialise new vis entries to 0 */
    memset(new_vis + cache_cap, 0, (new_cap - cache_cap));

    cache = new_cache;
    vis   = new_vis;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 1;

    ensure_cache(n);
    if (vis[n])
        return cache[n];

    seq_t result;
    switch (n & 3u)  /* n % 4 */
    {
        case 0: { /* n = 4*k */
            idx_t k = n >> 2;
            seq_t a_k   = a(k);
            seq_t a_2k  = a(k << 1);
            result = 2 * a_2k - a_k;
            break;
        }
        case 1: { /* n = 4*k + 1 */
            idx_t k = (n - 1) >> 2;
            seq_t a_k   = a(k);
            seq_t a_2k  = a(k << 1);
            seq_t a_2k1 = a((k << 1) + 1);
            result = 2 * a_2k + a_2k1;
            break;
        }
        case 2: { /* n = 4*k + 2 */
            idx_t k = (n - 2) >> 2;
            seq_t a_k   = a(k);
            seq_t a_2k  = a(k << 1);
            seq_t a_2k1 = a((k << 1) + 1);
            result = 2 * a_2k1 + a_2k;
            break;
        }
        case 3: { /* n = 4*k + 3 */
            idx_t k = (n - 3) >> 2;
            seq_t a_k   = a(k);
            seq_t a_2k  = a(k << 1);
            seq_t a_2k1 = a((k << 1) + 1);
            result = 2 * a_2k1 - a_k;
            break;
        }
    }

    cache[n] = result;
    vis[n] = 1;
    return result;
}

#endif  /* __cplusplus */
