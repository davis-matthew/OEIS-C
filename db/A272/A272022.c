/*
 * A272022  Look at the set of numbers obtained by permuting the digits of n
 *          in all possible ways, then remove n itself from the set. If the
 *          remaining numbers are all primes, then n is in the sequence.
 * https://oeis.org/A272022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 44, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n-th positive integer whose all
   non‑trivial permutations of its decimal digits are primes.
   The series is found by brute‑force search; this implementation
   memoises already‑found terms.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/get_digits.h"
#include "../utils/num_digits.h"
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

/* Comparator for sorting bytes. */
static int cmp_u8(const void *p1, const void *p2)
{
    return (*(const uint8_t *)p1) - (*(const uint8_t *)p2);
}

/* Recursive generation of all distinct permutations of the
   decimal digit array 'digits' (which must be sorted).
   Parameters:
   - digits[0..n-1] : sorted digits
   - n            : number of digits
   - pos          : depth of recursion (next position to fill)
   - used[0..n-1]  : flags saying whether a digit has been chosen
   - cur           : number built so far
   - orig          : original number (excluded from tests)
   - ok            : pointer to flag; set to false if a
                     non‑prime permutation is found
   - cnt           : pointer to counter of permutations examined
                     (only those different from orig). */
static void permute_rec(const uint8_t *digits, size_t n, size_t pos,
                        bool *used, seq_t cur, seq_t orig,
                        bool *ok, size_t *cnt)
{
    if (!*ok)
        return;
    if (pos == n) {
        if (cur != orig) {                 /* not the original itself */
            if (!is_prime_big(cur))
                *ok = false;
            (*cnt)++;
        }
        return;
    }
    uint8_t last = 255;                       /* sentinel for duplicate skipping */
    for (size_t i = 0; i < n; ++i) {
        if (!used[i] && digits[i] != last) {
            last = digits[i];
            used[i] = true;
            permute_rec(digits, n, pos + 1, used,
                        cur * 10 + digits[i], orig, ok, cnt);
            used[i] = false;
        }
    }
}

/* Returns true if x satisfies the sequence condition. */
static bool is_good(seq_t x)
{
    /* Single‑digit numbers have no non‑trivial permutations. */
    if (num_digits(x) <= 1)
        return false;

    size_t len = num_digits(x);
    uint8_t *digits = (uint8_t *)malloc(len);
    if (!digits)
        return false;

    size_t actual_len;
    get_digits(x, digits, &actual_len);
    len = actual_len;

    qsort(digits, len, sizeof(uint8_t), cmp_u8);

    bool *used = (bool *)calloc(len, sizeof(bool));
    if (!used) {
        free(digits);
        return false;
    }

    bool ok = true;
    size_t cnt = 0;
    permute_rec(digits, len, 0, used, (seq_t)0, x, &ok, &cnt);

    free(digits);
    free(used);
    return ok && cnt > 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* memoised sequence values      */
    static size_t cached_len = 0;   /* number of cached terms        */
    static size_t cache_cap = 0;    /* capacity of the cache array   */
    static idx_t next_candidate = 10;  /* start filtering from 10     */

    while (cached_len < n) {
        /* Find the next qualifying integer. */
        while (!is_good((seq_t)next_candidate))
            ++next_candidate;

        /* Grow the cache if necessary. */
        if (cached_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)  /* out of memory – terminate silently */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the newly found term. */
        cache[cached_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return cache[n - 1];
}
