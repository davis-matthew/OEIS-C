/*
 * A381897  a(n) = least integer m >= 2 such that prime(n) is a sum of the
 *          form Sum_{k>=0} floor(h/m^k) for some integer h >= 1.
 * https://oeis.org/A381897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer m ≥ 2 such that prime(n) can be written as
 *            Sum_{k≥0} floor(h / m^k) for some integer h ≥ 1. The function
 *            implements a simple search over m and a small range of h around
 *            an approximate value. The search always finds an m because at
 *            worst m = prime(n)−1 will work. The algorithm...
 * Range:     u8 n <= 45, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Get the k‑th prime, 1‑based indexing. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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

/* Helper: compute the sum Σ_{k≥0} ⌊h/m^k⌋ */
static seq_t floor_sum(seq_t h, int m)
{
    seq_t s = 0;
    for (seq_t t = h; t > 0; t /= m) {
        s += t;
    }
    return s;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);          /* prime(n) */
    for (int m = 2; ; ++m) {            /* search m */
        seq_t num     = (seq_t)p * (m - 1);
        seq_t h0      = num / m;        /* rough h ≈ p*(m-1)/m */
        seq_t low     = (h0 > 100) ? h0 - 100 : 1;
        seq_t high    = h0 + 100;       /* try a window of ±100 */

        for (seq_t h = low; h <= high; ++h) {
            if (floor_sum(h, m) == p) {
                return (seq_t)m;        /* found minimal m */
            }
        }
        /* If not found, continue to next m.  The loop will eventually
           terminate because at worst a very large m will succeed. */
    }
}
