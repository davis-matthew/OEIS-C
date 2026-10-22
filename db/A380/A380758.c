/*
 * A380758  Numbers which are not prime powers and their prime factors share a
 *          last digit in base 10.
 * https://oeis.org/A380758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 3535, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes OEIS A380758: numbers that are not prime powers
 * and whose distinct prime factors all share the same last digit.
 * The function searches integers in order, memoises the results,
 * and returns the *n*‑th term (a(1)=39, a(2)=69, ...).
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/_floor_isqrt_u64.h"   /* idx_t _floor_isqrt_u64(idx_t n); */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit,
                                         uint64_t **primes_out, size_t *cnt_out); */

/* Grow a seq_t array to hold at least 'need' elements.
   Uses realloc and returns the new capacity in *cap.          */
static void ensure_cap(size_t need, size_t *cap, seq_t **p)
{
    if (need <= *cap)
        return;
    size_t newcap = (*cap == 0) ? 16 : *cap * 2;
    while (newcap < need)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(*p, newcap * sizeof(seq_t));
    if (tmp == NULL)
        exit(EXIT_FAILURE);       /* allocation failure – abort */
    *p = tmp;
    *cap = newcap;
}

/* Test whether m is a composite number whose distinct prime factors
   all have the same last decimal digit.  Returns true only if
   at least two distinct prime factors exist.                 */
static bool good_number(uint64_t m, uint64_t *primes, size_t prime_cnt)
{
    if (m < 2)
        return false;

    uint64_t t = m;
    int    last_digit = -1;
    size_t distinct   = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > t)
            break;
        if (t % p == 0) {
            ++distinct;
            if (last_digit == -1)
                last_digit = (int)(p % 10);
            else if (last_digit != (int)(p % 10))
                return false;
            while (t % p == 0)
                t /= p;
        }
    }

    if (t > 1) {           /* remaining prime factor > 1 */
        ++distinct;
        if (last_digit == -1)
            last_digit = (int)(t % 10);
        else if (last_digit != (int)(t % 10))
            return false;
    }

    return distinct >= 2;   /* exclude primes and prime powers */
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                        /* offset is 1; 0 is unused */

    /* ===== Memoisation state ===== */
    static seq_t *cache  = NULL;      /* array of found terms */
    static size_t cache_cnt = 0;      /* number of terms stored   */
    static size_t cache_cap = 0;      /* array capacity           */

    static uint64_t *prime_arr = NULL;   /* primes for factorisation */
    static size_t   prime_cnt  = 0;      /* count of primes          */
    static uint64_t prime_lim  = 0;      /* highest prime computed   */

    static uint64_t last_checked = 1;    /* last integer examined    */

    /* Generate terms until we have a(n) */
    while (cache_cnt < (size_t)n) {
        ++last_checked;
        uint64_t val = last_checked;

        /* Ensure we have primes up to sqrt(val) */
        uint64_t needed = (uint64_t)_floor_isqrt_u64(val);
        if (needed > prime_lim) {
            /* Allocate new prime list and free the old one */
            uint64_t *new_primes = NULL;
            size_t   new_cnt = 0;
            build_primes_upto(needed, &new_primes, &new_cnt);
            free(prime_arr);          /* old array is no longer needed */
            prime_arr = new_primes;
            prime_cnt = new_cnt;
            prime_lim = needed;
        }

        if (good_number(val, prime_arr, prime_cnt)) {
            /* Store the term */
            ensure_cap(cache_cnt + 1, &cache_cap, &cache);
            cache[cache_cnt++] = (seq_t)val;
        }
    }

    return cache[n - 1];
}
