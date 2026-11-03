/*
 * A332076  Indices n of odd numbers 2n+1 such that k + 2^m is prime, where k
 *          and m are the odd part and 2-valuation, respectively, of 2n.
 * https://oeis.org/A332076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer n such that if 2n = k·2^m with k odd and
 *            m ≥ 1, then k + 2^m is prime. The function returns the n‑th such
 *            n
 * Range:     u8 n <= 62, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-03)
 * Helpers:   is_prime_u64, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#include "../utils/pow2.h"
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

static seq_t *indices = NULL;          /* cache of values satisfying the property */
static size_t  count   = 0;            /* number of cached values */
static size_t  cap     = 0;            /* allocated capacity of the cache */
static idx_t   last_checked = 0;       /* largest n examined so far */

/* Ensure the cache has at least ‘needed’ elements.  */
static void ensure_capacity(size_t needed)
{
    if (needed > cap) {
        size_t newcap = cap ? cap * 2 : 1;
        while (newcap < needed) newcap <<= 1;
        seq_t *newp = mem_resize(seq_t, indices, newcap);
        if (!newp) abort();            /* memory exhausted */
        indices = newp;
        cap     = newcap;
    }
}

/* Test whether 2n = k·2^m satisfies k + 2^m prime.  */
static bool property_holds(idx_t n)
{
    /* n ≥ 1 */
    /* trailing zeros in n give v₂(n) = t; then v₂(2n) = t + 1. */
    unsigned t = __builtin_ctzll((unsigned long long)n);
    idx_t m   = (idx_t)t + 1;      /* exponent of the power of 2 in 2n */
    idx_t k   = n >> t;            /* odd part of n */

    seq_t sum = ((seq_t)k) + pow2((idx_t)m);  /* k + 2^m */
    return is_prime_big(sum);
}

/* Return the n‑th index satisfying the condition.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* the sequence is defined for n ≥ 1 */

    while (count < n) {
        ++last_checked;           /* next integer to test */
        if (property_holds(last_checked)) {
            ensure_capacity(count + 1);
            indices[count++] = (seq_t)last_checked;
        }
    }
    return indices[n - 1];
}
