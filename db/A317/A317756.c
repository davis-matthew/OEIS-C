/*
 * A317756  Number of distinct primes obtained by cyclically shifting the
 *          decimal digits of the n-th prime.
 * https://oeis.org/A317756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of distinct primes among cyclic rotations of the
 *            n-th prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 6853 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

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

seq_t a(idx_t n)
{
    if (n == (idx_t)0) /* undefined for n = 0, return 0 */
        return (seq_t)0;

    /* Get the n-th prime (1‑based index). */
    uint32_t p = get_prime(n);

    /* Convert prime to string to obtain its decimal digits. */
    char buf[32];
    int len = snprintf(buf, sizeof buf, "%u", p);
    if (len <= 0)
        return (seq_t)0; /* safety check */

    /* Count distinct prime rotations. */
    seq_t count = 0;
    idx_t seen[20];
    size_t seen_cnt = 0;

    for (int shift = 0; shift < len; ++shift) {
        idx_t val = 0;
        for (int i = 0; i < len; ++i) {
            int d = buf[(shift + i) % len] - '0';
            val = val * 10 + d;
        }

        if (is_prime(val)) {
            bool present = false;
            for (size_t j = 0; j < seen_cnt; ++j) {
                if (seen[j] == val) {
                    present = true;
                    break;
                }
            }
            if (!present) {
                seen[seen_cnt++] = val;
                ++count;
            }
        }
    }
    return count;
}
