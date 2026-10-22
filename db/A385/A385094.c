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
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/get_prime.h"        /* get_prime(idx_t) */
#include "../utils/is_prime_small.h"   /* is_prime_small(uint64_t) */

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
        if (reach[s] && is_prime_small((uint64_t)s)) {
            if (buf_len >= buf_cap) {
                buf_cap = buf_cap ? buf_cap * 2 : 64;
                buf = (seq_t *)realloc(buf, buf_cap * sizeof(seq_t));
            }
            buf[buf_len++] = (seq_t)s;   /* automatically ascending */
        }
    }
    free(reach);

    _rep_primes = (seq_t *)malloc(buf_len * sizeof(seq_t));
    memcpy(_rep_primes, buf, buf_len * sizeof(seq_t));
    _rep_count  = buf_len;
    free(buf);

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
