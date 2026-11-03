/*
 * A245017  Numbers k such that (product of digits of k) + 1 and (product of
 *          digits of k)^2 + 1 are both prime.
 * https://oeis.org/A245017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 565, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9799 (2026-11-03)
 * Helpers:   digit_product, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements A245017: the n-th integer k such that
   (product_of_digits(k) + 1) is prime and (product_of_digits(k)^2 + 1)
   is also prime.  It memoises results so that calls arrive in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* static memoisation of found terms */
    static seq_t *terms = NULL;       /* array of found terms */
    static size_t cap   = 0;          /* capacity of terms array */
    static size_t len   = 0;          /* number of terms stored */
    static idx_t  checked = 0;        /* largest k examined so far */

    if (n == 0)                      /* offset is 1; undefined for n<1 */
        return (seq_t)0;

    /* If we already have the required term, return it immediately */
    if (n <= len)
        return terms[n - 1];

    /* Expand the search until we have at least n terms */
    while (len < n)
    {
        ++checked;
        seq_t k = (seq_t)checked;      /* current candidate */

        /* product of digits of k */
        seq_t p = digit_product(k);

        /* (p + 1) must be prime */
        seq_t p1 = p + (seq_t)1;
        if (!is_prime_big(p1))
            continue;

        /* (p^2 + 1) must be prime */
        seq_t p2 = p * p;
        seq_t p2plus = p2 + (seq_t)1;
        if (!is_prime_big(p2plus))
            continue;

        /* k satisfies the criterion – record it */
        append_candidate(&terms, &len, &cap, k);
    }

    return terms[n - 1];
}
