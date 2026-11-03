/*
 * A328118  Numbers k for which A276086(k) <= A002620(k), where A276086 is the
 *          primorial base exp-function and A002620(k) = floor(k^2/4).
 * https://oeis.org/A328118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 3608, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9283 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k where A276086(k) ≤ floor(k²/4). The function keeps an
 * incremental list of the qualifying k's and returns the n-th such k. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* ---- Helper to build primorials up to a given k -------------------------------- */
static uint64_t *primorials = NULL;   /* 0‑based, primorials[0] = 1 */
static size_t   primorials_cnt = 0;   /* number of primorials stored */

static void ensure_primorials(idx_t k)
{
    if (primorials_cnt == 0) {
        primorials = (uint64_t *)malloc(sizeof(uint64_t));
        primorials[0] = 1;               /* P0 = 1 */
        primorials_cnt = 1;
    }
    /* Extend until the last primorial > k (or until overflow) */
    while (primorials[primorials_cnt - 1] <= k) {
        idx_t next_prime = get_prime((idx_t)primorials_cnt);   /* 1‑based */
        unsigned __int128 prod = (unsigned __int128)primorials[primorials_cnt - 1] *
                                 (unsigned __int128)next_prime;
        if (prod > (unsigned __int128)ULLONG_MAX)
            break;   /* cannot extend further without overflow */
        primorials_cnt++;
        primorials = (uint64_t *)realloc(primorials,
                                         primorials_cnt * sizeof(uint64_t));
        primorials[primorials_cnt - 1] = (uint64_t)prod;
    }
}

/* ---- Compute whether a given k satisfies the inequality --------------------- */
static bool qualifies(idx_t k)
{
    ensure_primorials(k);

    /* Find largest index i such that P_i ≤ k */
    size_t i = primorials_cnt - 1;
    while (i > 0 && primorials[i] > k) i--;

    unsigned __int128 prod = 1;
    idx_t rem = k;
    unsigned __int128 limit = (unsigned __int128)k *
                               (unsigned __int128)k / 4;

    for (int pos = (int)i; pos >= 0; pos--) {
        uint64_t weight = primorials[(size_t)pos];
        idx_t q = rem / weight;
        rem -= q * weight;
        if (q) {
            idx_t prime = get_prime((idx_t)(pos + 1)); /* prime_{pos+1} */
            unsigned __int128 term = 1;
            for (idx_t e = q; e > 0; e--) term *= prime;
            prod *= term;
            if (prod > limit) return false;   /* early exit */
        }
    }
    return prod <= limit;
}

/* ---- Public sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *seq  = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t next_k = 1;      /* next integer to test */

    while (seq_len < n) {
        if (qualifies(next_k)) {
            grow_seq_array(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return seq[n - 1];
}
