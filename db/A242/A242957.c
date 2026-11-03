/*
 * A242957  Consider a number n with m decimal digits. The sequence lists the
 *          numbers n having the suffix of length m-1 in the middle of the
 *          decimal expansion of prime(n).
 * https://oeis.org/A242957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A242957.
   The sequence lists integers n such that the suffix of n obtained by
   discarding the most significant digit appears in the middle of the
   decimal expansion of the n‑th prime.  The implementation tests
   candidates in increasing order and stops when the desired index has
   been reached.  It works for any seq_t type specified by the build. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits_idx.h"

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Number of decimal digits of a seq_t value (handles arbitrary size). */
static int seq_digit_len(seq_t x)
{
    if (x == 0) return 1;
    int len = 0;
    seq_t ten = (seq_t)10;
    while (x != 0) {
        x /= ten;
        ++len;
    }
    return len;
}

/* Return true if n satisfies the defining property. */
static bool qualifies(idx_t n)
{
    /* At least two decimal digits required so that a suffix of length m-1
       can exist. */
    int m = num_digits_idx(n);
    if (m <= 1) return false;

    int suffix_len = m - 1;
    seq_t pow_suf_len = pow10((idx_t)suffix_len);      /* 10^(m-1) */
    seq_t suffix_num = (seq_t)n % pow_suf_len;          /* numeric suffix */

    /* Length of the numeric suffix (leading zeros do not count). */
    int s_len;
    if (suffix_num == 0) {
        s_len = 1;
    } else {
        s_len = seq_digit_len(suffix_num);
    }

    /* Fetch the n-th prime (1‑based). */
    seq_t prime = get_prime(n);
    int prime_len = seq_digit_len(prime);

    /* The suffix must fit inside the prime and be centered. */
    if (prime_len < s_len) return false;
    if ((prime_len - s_len) % 2 != 0) return false;          /* centered? */

    int pos = (prime_len - s_len) / 2;                      /* start index */
    seq_t pow_s_len = pow10((idx_t)s_len);                  /* 10^s_len   */
    seq_t divisor   = pow10((idx_t)pos);                    /* 10^pos     */

    seq_t part = (prime / divisor) % pow_s_len;             /* substring */

    return part == suffix_num;
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;

    while (count < n) {
        if (qualifies(candidate)) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }

    /* Should never be reached for valid n. */
    return (seq_t)0;
}
