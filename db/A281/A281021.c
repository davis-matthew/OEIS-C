/*
 * A281021  Primes k with (say) j digits that produce two other primes, x and
 *          y, from the sums x = Sum_{i=1..j} (k mod 10^i) and y =
 *          Sum_{i=0..j-1} (floor(k/10^i)).
 * https://oeis.org/A281021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes OEIS A281021:
   For each n≥1, a(n) is the n‑th prime k such that
   x = Σ_{i=1..j} (k mod 10^i)
   y = Σ_{i=0..j-1} ⌊k/10^i⌋
   are both prime (j = number of decimal digits of k). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>          /* for memcpy in append_value */

/* Utility headers supplied by the build system */
#include "../utils/is_prime.h"
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

/* Dynamic array helper ----------------------------------------------------- */
static void append_value(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Check whether a prime k satisfies the OEIS condition. ------------------- */
static bool satisfies_k(idx_t k)
{
    /* Extract decimal digits LSB first. */
    uint8_t digits[32];
    idx_t t = k, j = 0;
    while (t > 0) {
        digits[j++] = (uint8_t)(t % 10);
        t /= 10;
    }
    if (j == 0)                /* no prime has 0 digits – defensive */
        return false;

    /* Compute x = sum of suffixes. */
    seq_t suffix = 0ULL;
    seq_t power  = 1ULL;
    seq_t x      = 0ULL;
    for (idx_t i = 0; i < j; ++i) {
        suffix += (seq_t)digits[i] * power;
        x      += suffix;
        power  *= 10ULL;
    }

    /* Compute y = sum of prefixes. */
    seq_t y      = 0ULL;
    seq_t prefix = (seq_t)k;
    while (prefix > 0) {
        y += prefix;
        prefix /= 10ULL;
    }

    return is_prime_big(x) && is_prime_big(y);
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1; undefined for n<1 */

    /* Static cache of discovered terms and the next candidate. */
    static seq_t *terms   = NULL;
    static size_t  tlen   = 0;
    static size_t  tcap   = 0;
    static idx_t   cand   = 2;  /* next number to test for primality */

    while (tlen < n) {
        /* Find next prime. */
        for (;;) {
            if (cand == 2 || (cand > 2 && (cand & 1U) != 0)) {
                if (is_prime(cand))
                    break;
            }
            cand++;
        }

        /* Test the OEIS property. */
        if (satisfies_k(cand))
            append_value(&terms, &tlen, &tcap, (seq_t)cand);

        cand++;   /* advance for next call */
    }

    return terms[n - 1];
}
