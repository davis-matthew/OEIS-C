/*
 * A370523  Numbers k > 2 such that all positive values of k - 2^(2^m) are
 *          prime, with integer m >= 0.
 * https://oeis.org/A370523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A370523:
   Numbers k > 2 such that for every m ≥ 0 with 2^(2^m) < k,
   the difference k – 2^(2^m) is prime.  The function searches for
   such numbers incrementally and caches already found terms. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
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

static seq_t *terms   = NULL;   /* cache of found terms               */
static size_t terms_cap = 0;    /* allocated capacity of the cache    */
static size_t terms_len = 0;    /* current number of stored terms     */
static seq_t last_k = 3;        /* last k that was tested             */

static void ensure_terms_cap(size_t need)
{
    if (need > terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        terms = mem_resize(seq_t, terms, new_cap);
        terms_cap = new_cap;
    }
}

static void add_term(seq_t v)
{
    ensure_terms_cap(terms_len + 1);
    terms[terms_len++] = v;
}

/* Test whether a candidate k satisfies the definition.  */
static bool is_valid(seq_t k)
{
    if (k <= 2) return false;

    seq_t p = 2;                           /* 2^(2^0) */
    while (p < k) {
        seq_t diff = k - p;
        if (!is_prime_big(diff)) return false;
        /* next power: (2^(2^m))^2 = 2^(2^(m+1)) */
        if (p > k / p) break;             /* next square would be ≥ k */
        p = p * p;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* undefined for n < 1 */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        ++last_k;
        if (is_valid(last_k))
            add_term(last_k);
    }
    return terms[n - 1];
}
