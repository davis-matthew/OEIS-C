/*
 * A354569  Ordered even leg lengths k (listed with multiplicity) of primitive
 *          Pythagorean triangles such that all odd prime factors of k are
 *          congruent to 1 (mod 4) and at least one prime factor is congruent
 *          to 1 (mod 4).
 * https://oeis.org/A354569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   good_odd_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A354569: ordered even leg lengths of primitive
   Pythagorean triangles whose odd prime factors are all 1 (mod 4) and
   at least one such prime occurs.  The function generates all
   Pythagorean pairs (m,n) with m>n, gcd(m,n)=1, m-n odd,
   checks the prime–factor condition on m and n, and collects
   2*m*n.  The collection is kept sorted so that duplicates are
   preserved.  The sequence values are returned in the required index
   order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/good_odd_primes.h"

/* ---------- helper functions ---------------------------------------- */

static uint64_t gcd_uint64(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t t = a % b;
        a = b; b = t;
    }
    return a;
}

/* Comparator for qsort on seq_t array */
static int seq_cmp(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* ---------- global state --------------------------------------------- */

static seq_t *data      = NULL;    /* sorted list of k values           */
static size_t data_len  = 0;
static size_t data_cap  = 0;

static uint8_t *ok_cache = NULL;  /* 0: unknown, 1: true, 2: false */
static uint64_t cache_size = 0;
static uint64_t m_max = 0;        /* largest m processed so far       */

/* Append a new k value, growing the array if necessary */
static void add_value(seq_t v) {
    if (data_len == data_cap) {
        data_cap = data_cap ? data_cap * 2 : 16;
        data = (seq_t *)realloc(data, data_cap * sizeof(seq_t));
    }
    data[data_len++] = v;
}

/* Ensure ok_cache covers indices up to n (inclusive) */
static void ensure_ok_cache(uint64_t n) {
    if (n < cache_size) return;
    uint64_t new_size = n + 1;
    ok_cache = (uint8_t *)realloc(ok_cache, new_size);
    for (uint64_t i = cache_size; i <= n; ++i) {
        ok_cache[i] = good_odd_primes(i) ? 1u : 2u;
    }
    cache_size = new_size;
}

/* Extend the search to all m <= new_limit, adding corresponding k's. */
static void extend_to(uint64_t new_limit) {
    if (new_limit <= m_max) return;
    ensure_ok_cache(new_limit);
    for (uint64_t m = m_max + 1; m <= new_limit; ++m) {
        bool m_ok = ok_cache[m] == 1u;                      /* all odd primes ≡1 mod4 */
        bool m_has_odd = (m & (m - 1)) != 0;                /* not a power of two */

        if (!m_ok) continue;                                /* skip bad m */

        for (uint64_t n = 1; n < m; ++n) {
            if (((m - n) & 1u) == 0) continue;             /* m-n odd */
            if (gcd_uint64(m, n) != 1u) continue;           /* gcd=1 */

            bool n_ok = ok_cache[n] == 1u;
            if (!n_ok) continue;                           /* skip bad n */

            bool n_has_odd = (n & (n - 1)) != 0;            /* not a power of two */
            /* At least one odd prime in k = 2*m*n */
            if (!m_has_odd && !n_has_odd) continue;

            unsigned __int128 tmp = (unsigned __int128)2 * m * n;
            seq_t k = (seq_t)tmp;
            add_value(k);
        }
    }
    /* Keep the list sorted; duplicates are preserved. */
    if (data_len > 1)
        qsort(data, data_len, sizeof(seq_t), seq_cmp);

    m_max = new_limit;
}

/* Ensure that the array contains at least target terms. */
static void ensure_size(idx_t target) {
    if (target == 0) return;
    if (data == NULL) {
        data_cap = 16;
        data = (seq_t *)malloc(data_cap * sizeof(seq_t));
    }
    if (ok_cache == NULL) {
        ok_cache = (uint8_t *)malloc(1);
        ok_cache[0] = 1u;       /* unused, but safe */
        cache_size = 1;
    }
    /* Sometimes future m can produce a k smaller than current data;
       we estimate the minimal possible future k. */
    while (true) {
        if (data_len >= (size_t)target) {
            uint64_t future_m = m_max + 1;
            uint64_t n_min = (future_m & 1u) ? 2u : 1u;
            unsigned __int128 future_min = (unsigned __int128)2 * future_m * n_min;
            seq_t target_val = data[target - 1];
            if ((seq_t)future_min > target_val) break;
        }
        /* Need more terms: extend search. */
        uint64_t new_limit = m_max + 500;   /* step size; adjustable */
        extend_to(new_limit);
    }
}

/* ---------- public function ----------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset 1; never used in tests */
    ensure_size(n);
    return data[n - 1];
}
