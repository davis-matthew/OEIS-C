/*
 * A328614  Number of 1-digits in primorial base expansion of n.
 * https://oeis.org/A328614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the number of 1‑digits in the primorial base expansion of
 *            n
 * Range:     u8 n <= 2318, u16 n <= 32768, u32 n <= 32768, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 122 terms through n = 9740 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

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

static seq_t *primorial_vals = NULL;  /* primorial[0] = 1, primorial[i] = product of first i primes */
static size_t prim_cap   = 0;          /* allocated size */
static size_t prim_len   = 0;          /* number of primorial entries */

/* Ensure primorial_vals contains all primorials ≤ max_n (in idx_t). */
static void ensure_primorial(idx_t max_n)
{
    /* Grow until the last primorial exceeds max_n or we have at least one entry. */
    while (prim_len == 0 || primorial_vals[prim_len-1] <= (seq_t)max_n) {
        /* Increase capacity if needed. */
        if (prim_len >= prim_cap) {
            size_t new_cap = prim_cap ? prim_cap * 2 : 8;
            seq_t *new_arr;
#ifdef __cplusplus
            new_arr = new seq_t[new_cap];
            for (size_t i = 0; i < prim_len; ++i)
                new_arr[i] = primorial_vals[i];
            delete[] primorial_vals;
#else
            new_arr = realloc(primorial_vals, new_cap * sizeof(seq_t));
            /* realloc keeps existing data */
#endif
            primorial_vals = new_arr;
            prim_cap = new_cap;
        }

        /* Append the next primorial.  primorial_vals[0] = 1. */
        if (prim_len == 0) {
            primorial_vals[0] = (seq_t)1;
        } else {
            uint32_t p = get_prime((idx_t)prim_len);   /* next prime */
            primorial_vals[prim_len] = primorial_vals[prim_len-1] * (seq_t)p;
        }
        ++prim_len;
    }
}

/* Number of 1‑digits in the primorial base expansion of n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* a(0) = 0 */

    ensure_primorial(n);

    /* Find largest k with primorial_vals[k] ≤ n. */
    size_t k = prim_len - 1;
    while (k > 0 && primorial_vals[k] > (seq_t)n)
        --k;

    seq_t remainder = (seq_t)n;
    int count = 0;

    /* Subtract multiples of primorials from highest to lowest (excluding 1). */
    for (; k > 0; --k) {
        seq_t q = remainder / primorial_vals[k];
        if (q == (seq_t)1)
            ++count;
        remainder -= q * primorial_vals[k];
    }

    /* Base unit (prime[0] = 2, primorial[0] = 1).  Count a remaining 1. */
    if (remainder == (seq_t)1)
        ++count;

    return (seq_t)count;
}
