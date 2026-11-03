/*
 * A267378  Primes of the form k^4 - k^2 + 7.
 * https://oeis.org/A267378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A267378 – primes of the form k^4 – k^2 + 7.
   a(n) = the nth prime value of k^4 – k^2 + 7 for k ≥ 0 (duplicates skipped). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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
    if (n == 0) {                     /* Off‑the‑offset calls return 0 */
        return 0;
    }

    static seq_t *prime_list = NULL;  /* growing array of primes found so far   */
    static size_t prime_len  = 0;     /* number of primes stored                */
    static size_t prime_cap  = 0;     /* capacity of prime_list
                                        (append_candidate handles growth) */

    static idx_t next_k      = 0;     /* next k to evaluate                     */
    static seq_t last_val    = 0;     /* value of previous k to skip duplicates  */
    static bool  has_last    = false;

    while (prime_len < (size_t)n) {
        seq_t kk   = (seq_t)next_k;       /* promote k to seq_t */
        seq_t ksq  = kk * kk;             /* k²                */
        seq_t val  = ksq * ksq - ksq + (seq_t)7;   /* k⁴ – k² + 7 */

        bool dup = has_last && val == last_val;
        if (!dup) {
            if (is_prime_big(val)) {
                append_candidate(&prime_list, &prime_len, &prime_cap, val);
            }
            last_val = val;
            has_last = true;
        }
        ++next_k;
    }

    return prime_list[(size_t)n - 1];
}
