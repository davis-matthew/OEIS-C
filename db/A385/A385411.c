/*
 * A385411  Numbers k that are not perfect powers, not divisible by some b,
 *          and whose trailing digits form a power of b, where 1 < b < k.
 * https://oeis.org/A385411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth term of OEIS A385411
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_clamped
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A term k is valid if it is

   * >= 10,
   * not a perfect power,
   * there exists a base 1 < b < k such that
     k is not divisible by b and the decimal suffix of k equals b^e
     for some e ≥ 0 (b^0 == 1 is allowed),

   In short: a number k has a trailing block that is a power of a base
   which does not divide k.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow_clamped.h"

/* auxiliary functions ----------------------------------------------------- */

static inline unsigned bitlen_seq_t(seq_t v)
{
    unsigned len = 0;
    while (v) {
        v >>= 1;
        ++len;
    }
    return len;
}

/* 10^e as seq_t */
static seq_t pow10(unsigned e)
{
    seq_t r = 1;
    for (unsigned i = 0; i < e; ++i) {
        r *= 10;
    }
    return r;
}

/* decimal digit count, treating 0 as 1 digit */
static int digit_len(seq_t x)
{
    int len = 0;
    do {
        ++len;
        x /= 10;
    } while (x);
    return len;
}

/* floor n^(1/e) for integer e >= 1 */
static seq_t integer_nth_root(seq_t n, unsigned e)
{
    if (e <= 1)
        return n;

    int d = digit_len(n);
    unsigned ceil_digits = (d + e - 1) / e;          /* ceil(d/e) */
    seq_t high = pow10(ceil_digits);                 /* 10^(ceil(d/e)) */
    seq_t low  = 1;

    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        seq_t p   = pow_clamped(mid, e, n);
        if (p <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* test whether x is a perfect power (exponent >= 2) */
static bool is_perfect_power(seq_t x)
{
    if (x < 4)          /* 2^2 = 4 is the smallest non‑trivial power */
        return false;

    unsigned max_e = bitlen_seq_t(x) - 1;
    for (unsigned e = 2; e <= max_e; ++e) {
        seq_t root = integer_nth_root(x, e);
        if (root <= 1)
            continue;
        seq_t p = pow_clamped(root, e, x);
        if (p == x)
            return true;
    }
    return false;
}

/* does suffix s represent a power of some base b (b > 1, b < k)
   such that k % b != 0?  For s == 1 this is always true. */
static bool suffix_is_power(seq_t s, seq_t k)
{
    if (s == 0)
        return false;
    if (s == 1)
        return true;                     /* e = 0, base arbitrary, non‑dividing exists */

    /* e = 1 : power base = s itself */
    if (s < k && k % s != 0)
        return true;

    /* e >= 2 */
    unsigned max_e = bitlen_seq_t(s) - 1;
    for (unsigned e = 2; e <= max_e; ++e) {
        seq_t root = integer_nth_root(s, e);
        if (root <= 1)
            continue;
        seq_t p = pow_clamped(root, e, s);
        if (p != s)
            continue;
        if (root < k && k % root != 0)
            return true;
    }
    return false;
}

/* candidate test for a single k */
static bool candidate_k(seq_t k)
{
    if (k < 10)                /* single digit numbers cannot be terms */
        return false;

    if (is_perfect_power(k))
        return false;

    int digits = digit_len(k);
    seq_t pow10_div = 10;       /* 10^1 */

    for (int l = 1; l <= digits; ++l) {
        seq_t suffix = k % pow10_div;
        if (suffix_is_power(suffix, k))
            return true;

        if (l == digits)        /* avoid unnecessary overflow */
            break;
        pow10_div *= 10;         /* next 10^l */
    }
    return false;
}

/* dynamic table of found terms ------------------------------------------- */
static seq_t *terms      = NULL;
static size_t terms_len  = 0;
static size_t terms_cap  = 0;
static seq_t last_k      = 0;

/* make sure we have at least n terms */
static void ensure_terms(idx_t n)
{
    if (terms_len >= (size_t)n)
        return;

    if (terms_cap == 0) {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms)
            exit(EXIT_FAILURE);
    }

    while (terms_len < (size_t)n) {
        ++last_k;
        if (candidate_k(last_k)) {
            if (terms_len >= terms_cap) {
                size_t new_cap = terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = last_k;
        }
    }
}

/* sequence function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for 0, but guard */

    ensure_terms(n);
    return terms[n - 1];
}
