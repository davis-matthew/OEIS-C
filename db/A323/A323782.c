/*
 * A323782  Prime numbers such that the reverse of the balanced ternary
 *          representation is a prime or a negated prime.
 * https://oeis.org/A323782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Gen: primes p such that the reverse of the balanced ternary
 * representation of p is a prime or a negative prime.
 * The function is memoised and works for any order of calls.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
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
    if (n == 0) {
        return (seq_t)0; /* undefined for n < 1, safe fallback */
    }

    /* Memoisation state */
    static seq_t *terms = NULL;      /* array of found terms      */
    static size_t terms_len = 0;     /* number of stored terms    */
    static size_t terms_cap = 0;     /* capacity of the array     */
    static seq_t cand_start = (seq_t)2; /* next candidate to test */

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* Find the next prime candidate */
        seq_t cand = cand_start;
        while (!is_prime_big(cand)) {
            ++cand;
        }
        cand_start = cand + (seq_t)1;   /* next search starts here */

        /* Convert cand to balanced ternary digits (least‑significant first) */
        int8_t digits[256];   /* enough for even 128‑bit numbers */
        size_t dlen = 0;
        seq_t tmp = cand;
        while (tmp != (seq_t)0) {
            int rem = (int)(tmp % (seq_t)3);
            tmp = tmp / (seq_t)3;
            if (rem == 2) {
                rem = -1;
                ++tmp;
            }
            digits[dlen++] = (int8_t)rem;
        }
        if (dlen == 0) {
            digits[dlen++] = (int8_t)0;
        }

        /* Build the reversed balanced ternary number (most‑significant first) */
        __int128 rev_val = 0;   /* signed to handle negative values */
        for (size_t i = 0; i < dlen; ++i) {
            rev_val = rev_val * 3 + digits[i];
        }
        seq_t abs_rev = rev_val < 0 ? -(seq_t)rev_val : (seq_t)rev_val;

        /* Accept the prime if |reverse| is a prime > 1 */
        if (abs_rev > 1 && is_prime_big(abs_rev)) {
            append_candidate(&terms, &terms_len, &terms_cap, cand);
        }
    }

    return terms[n - 1];
}
