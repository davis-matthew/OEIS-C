/*
 * A385094  Primes that are the sum of distinct positive cubes.
 * https://oeis.org/A385094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n+421) for n > 1027 a(n) = prime that can be
 *            expressed as a sum of distinct positive cubes for n ≤ 1027
 * Range:     u8 n <= 3, u16 n <= 1030, u32 n <= 1030, u64 n <= 1030,
 *            u128 n <= 1030, big any n
 * Validated: standard, pass, 69 terms through n = 1014 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <stddef.h>
#include "../utils/is_prime_seq.h"
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

/* Cached representable primes (all up to our LIMIT). */
static bool                _seq_initialized = false;
static seq_t *             _rep_primes      = NULL;   /* sorted ascending */
static size_t              _rep_count       = 0;      /* number of elements stored */

/* Helper: build the list of representable primes up to at least the
   1027‑th element.  We use a DP to enumerate all sums of distinct
   positive cubes up to a generous LIMIT (200 000).  All primes > 12 101
   are representable, so the 1027‑th term lies well within this bound. */
static void _build_representable_primes(void)
{
    const idx_t LIMIT = 200000;          /* 200 000 > 12101 and enough for 1027 terms */

    /* Compute all cube values ≤ LIMIT. */
    size_t cube_cnt = 0;
    idx_t  cubes[64];
    for (idx_t k = 1; ; ++k) {
        idx_t cube = k * k * k;
        if (cube > LIMIT) break;
        cubes[cube_cnt++] = cube;
    }

    /* DP for subset sums of cubes. */
    bool *reach = (bool *)calloc((size_t)LIMIT + 1, sizeof(bool));
    reach[0] = true;
    for (size_t ci = 0; ci < cube_cnt; ++ci) {
        idx_t c = cubes[ci];
        /* unsigned decreasing loop with termination on 0 */
        for (idx_t s = LIMIT - c; ; --s) {
            if (reach[s]) reach[s + c] = true;
            if (s == 0) break;
        }
    }

    /* Collect all reachable sums that are prime. */
    seq_t *buf     = NULL;
    size_t buf_cap = 0, buf_len = 0;
    for (idx_t s = 2; s <= LIMIT; ++s) {
        if (reach[s] && is_prime_seq((uint64_t)s)) {
            if (buf_len >= buf_cap) {
                buf_cap = buf_cap ? buf_cap * 2 : 64;
                buf = mem_resize(seq_t, buf, buf_cap);
            }
            buf[buf_len++] = (seq_t)s;   /* automatically ascending */
        }
    }
    free(reach);

    _rep_primes = mem_new(seq_t, buf_len);
    mem_copy(_rep_primes, buf, buf_len * sizeof(seq_t));
    _rep_count  = buf_len;
    mem_free(buf);

    _seq_initialized = true;
}

seq_t a(idx_t n)
{
    if (!_seq_initialized) {
        _build_representable_primes();
    }

    if (n <= _rep_count) {
        /* 1‑based indexing into pre‑computed array */
        return _rep_primes[n - 1];
    }

    /* For all larger n use the proven asymptotic formula */
    return get_prime(n + 421);
}
