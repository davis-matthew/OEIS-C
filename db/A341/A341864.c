/*
 * A341864  Least increasing sequence of primes a(n) == A020652(n) (mod
 *          A038567(n)).
 * https://oeis.org/A341864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 436, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9837 (2026-11-03)
 * Helpers:   gcd, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = least prime > a(n-1) congruent to A020652(n) (mod A038567(n))
   where A020652/A038567 enumerates fractions < 1 in lowest terms by increasing
   denominator then numerator.  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *seq_a      = NULL;          /* cache for computed a(n) values */
static size_t seq_a_cap  = 0;             /* current capacity of seq_a  */
static idx_t seq_a_last  = 0;             /* number of a(n) values stored */

/* Enumeration state for fractions < 1 (coprime numerator/denominator) */
static idx_t cur_q = 2;   /* current denominator */
static idx_t cur_p = 1;   /* candidate numerator for cur_q */

/* Ensure that seq_a has capacity at least n */
static void ensure_seq_a_size(idx_t n)
{
    if (n > seq_a_cap) {
        size_t new_cap = seq_a_cap ? seq_a_cap * 2 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = seq_realloc(seq_a, seq_a_cap, new_cap);
        seq_a = tmp;
        seq_a_cap = new_cap;
    }
}

/* Find the least prime > prev that satisfies cand ≡ p (mod q) */
static seq_t find_next_prime_mod(seq_t prev, seq_t p, seq_t q)
{
    seq_t rem = prev % q;
    seq_t offset = (p - rem + q) % q;
    seq_t cand = prev + offset;
    if (cand <= prev) /* safety: ensure strictly greater than prev */
        cand += q;
    while (!is_prime_big(cand))
        cand += q;
    return cand;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* offset 1: a(1) is the first term */

    /* If already computed, return cached value */
    if (n <= seq_a_last)
        return seq_a[n - 1];

    /* Compute values up to n */
    for (idx_t k = seq_a_last + 1; k <= n; ++k) {
        /* Enumerate next coprime fraction p/q */
        seq_t p, q;
        for (;;) {
            /* Ensure cur_p < cur_q and coprime */
            while (cur_p < cur_q) {
                if (gcd((seq_t)cur_p, (seq_t)cur_q) == (seq_t)1)
                    break;
                ++cur_p;
            }
            if (cur_p < cur_q) {
                p = (seq_t)cur_p;
                q = (seq_t)cur_q;
                ++cur_p;   /* advance for next call */
                break;
            }
            /* Move to next denominator */
            ++cur_q;
            cur_p = 1;
        }

        seq_t prev = (k == 1) ? (seq_t)0 : seq_a[k - 2];
        seq_t val  = find_next_prime_mod(prev, p, q);

        ensure_seq_a_size(k);
        seq_a[k - 1] = val;
    }

    seq_a_last = n;
    return seq_a[n - 1];
}
