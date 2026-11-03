/*
 * A242255  Primes-greedy summable primes, as defined at A242252.
 * https://oeis.org/A242255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 69 terms through n = 480 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate primes incrementally, test each prime p with the greedy
 * summation algorithm: let r = p, repeatedly subtract the largest prime
 * strictly smaller than p that is ≤ r (after the first step r < p).  If r
 * becomes 0 the prime is added to the sequence.  The function memoizes all
 * computed sequence values and all generated primes, so repeated calls with
 * arbitrary n are fast. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* Internal helper: binary search the last element ≤ limit in a sorted array. */
static
size_t binary_search_last_le(const seq_t *arr, size_t len, seq_t limit)
{
    if (len == 0) return (size_t)-1;
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] <= limit) lo = mid + 1;
        else hi = mid;
    }
    if (lo == 0) return (size_t)-1;
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Static storage for primes and sequence values. */
    static seq_t *primes = NULL;
    static size_t prime_len = 0, prime_cap = 0;
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static seq_t last_checked = 0;

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        /* Find the next prime after last_checked. */
        seq_t cand = last_checked + 1;
        while (!is_prime_big(cand)) ++cand;

        /* Append prime to the primes list. */
        ensure_capacity(prime_len + 1, &prime_cap, OEISC_VPP(&primes),
                        sizeof(seq_t));
        primes[prime_len++] = cand;
        last_checked = cand;

        /* Test whether cand is a primes‑greedy summable prime. */
        seq_t p = cand;
        seq_t rem = p;
        bool ok = true;
        while (rem > 0) {
            if (rem < 2) { ok = false; break; }
            seq_t limit = (rem == p) ? (p - 1) : rem;
            size_t idx = binary_search_last_le(primes, prime_len, limit);
            if (idx == (size_t)-1) { ok = false; break; }
            rem -= primes[idx];
        }
        if (ok) {
            ensure_capacity(seq_len + 1, &seq_cap, OEISC_VPP(&seq),
                            sizeof(seq_t));
            seq[seq_len++] = p;
        }
    }

    return seq[n - 1];
}
