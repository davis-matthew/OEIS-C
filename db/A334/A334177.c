/*
 * A334177  Squarefree part of numerator of the squared area of the Heronian
 *          triangle with sequential prime sides whose shortest leg is
 *          prime(n).
 * https://oeis.org/A334177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Formula: a(n) = squarefree part of the numerator of the squared area
 *             of the Heronian triangle with sequential prime sides
 *             (p_n, p_{n+1}, p_{n+2}), where p_k is the k-th prime.
 *             For each n≥1, let
 *                 s = (p_n + p_{n+1} + p_{n+2}) / 2
 *                 A² = s*(s-p_n)*(s-p_{n+1})*(s-p_{n+2})
 *             then a(n) = squarefree_part( numerator of A² after reduction ).
 *  This implementation factors all terms individually and accumulates
 *  the parity of each prime factor to obtain the squarefree part.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <stdlib.h>
#include "../utils/factor_u64.h"

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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    if (n == 1)       /* degenerate triangle {2,3,5} */
        return 0;

    /* primes of the triangle */
    uint64_t p = get_prime(n);
    uint64_t q = get_prime(n + 1);
    uint64_t r = get_prime(n + 2);

    /* the four factors in the numerator of A² = product / 16 */
    uint64_t term[4];
    term[0] = p + q + r;          /* p+q+r */
    term[1] = q + r - p;          /* q+r-p   */
    term[2] = p + r - q;          /* p+r-q   */
    term[3] = p + q - r;          /* p+q-r   */

    /* acceleration: if any term is 0, area is 0 */
    if (term[3] == 0)
        return 0;

    /* parity table for prime factors up to 400000 (safe limit) */
    static bool parity[400001] = { 0 };
    /* list of primes that were toggled during the current call */
    static uint32_t used[256];
    size_t used_cnt = 0;

    for (int i = 0; i < 4; ++i) {
        uint64_t v = term[i];
        if (v <= 1)          /* skip 0 or 1 */
            continue;

        /* factor v into prime powers */
        uint64_t pr[20];
        uint64_t ex[20];
        size_t cnt;
        factor_u64(v, pr, ex, &cnt);

        for (size_t j = 0; j < cnt; ++j) {
            if (ex[j] & 1U) {                /* odd exponent -> toggle */
                uint32_t prime = (uint32_t)pr[j];
                if (!parity[prime]) {
                    parity[prime] = true;
                    used[used_cnt++] = prime;
                } else {
                    parity[prime] = false;
                }
            }
        }
    }

    /* calculate the product of primes with odd parity */
    seq_t res = 1;
    for (size_t i = 0; i < used_cnt; ++i) {
        if (parity[used[i]])                /* still odd after all toggles */
            res = mul_small(res, used[i]);

        parity[used[i]] = false;            /* reset for next call */
    }

    return res;
}
