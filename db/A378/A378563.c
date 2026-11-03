/*
 * A378563  Primes that remain prime if any three of their digits are deleted.
 * https://oeis.org/A378563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search: successive primes are tested for the
   “delete any three digits” property.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"
/* Utilities provided by the framework */

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

/* Count decimal digits of a seq_t number (no external helper needed). */
static int seq_digit_len(seq_t n)
{
    if (n == 0) return 1;
    int len = 0;
    while (n > 0) {
        n /= 10;
        ++len;
    }
    return len;
}

/* Return true iff prime p has the mentioned deletion property.      */
static bool property_holds(seq_t p)
{
    /* Extract digits into an array in most‑significant order. */
    int len = seq_digit_len(p);
    if (len < 4) return false;          /* must have at least four digits */

    uint8_t msd[64];                    /* sufficient for 128‑bit numbers */
    seq_t tmp = p;
    for (int i = 0; i < len; ++i) {      /* store reversed */
        msd[i] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    /* Reverse into msd[0..len‑1] */
    for (int i = 0, j = len - 1; i < j; ++i, --j) {
        uint8_t t = msd[i];
        msd[i] = msd[j];
        msd[j] = t;
    }

    /* Enumerate all ways to delete three digits. */
    for (int a = 0; a < len - 2; ++a)
        for (int b = a + 1; b < len - 1; ++b)
            for (int c = b + 1; c < len; ++c) {
                seq_t val = 0;
                for (int i = 0; i < len; ++i) {
                    if (i == a || i == b || i == c) continue;
                    val = val * 10 + msd[i];
                }
                if (!is_prime_big(val)) return false;
            }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

    /* Static storage for memoised terms. */
    static seq_t *terms   = NULL;
    static size_t term_len = 0;
    static size_t term_cap = 0;

    /* Grow capacity helper. */
    if (term_len + 1 > term_cap) {
        size_t new_cap = term_cap ? term_cap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, terms, new_cap);
        if (!tmp) return 0;       /* allocation failure */
        terms = tmp;
        term_cap = new_cap;
    }

    /* If we already have the requested term, return it. */
    if (term_len >= n) return terms[n - 1];

    /* Search for the next prime that satisfies the property. */
    seq_t candidate = (n == 1) ? 2237 : terms[term_len - 1] + 2; /* start */
    while (term_len < n) {
        /* Ensure candidate is odd and >=2 */
        if (candidate < 2) candidate = 2;
        else if ((candidate & 1) == 0) ++candidate;

        /* Test primality of the candidate. */
        if (!is_prime_big(candidate)) { candidate += 2; continue; }

        if (property_holds(candidate)) {
            terms[term_len++] = candidate;
        }
        candidate += 2;
    }

    return terms[n - 1];
}
