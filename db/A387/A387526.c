/*
 * A387526  Positive numbers k with property that by doubling a substring in
 *          place we get a prime.
 * https://oeis.org/A387526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is found by brute‑force search. For each integer k we test
 *            all substrings that do not start with 0, double the numeric
 *            value of the substring, replace it in k, and check if the
 *            resulting number is prime. The first n numbers satisfying this
 *            property are stored in a static array for fast futur...
 * Range:     u8 n <= 27, u16 n <= 2575, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9850 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- helpers ---------- */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

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

static bool has_prime_substring(seq_t k)
{
    /* Convert k to a decimal string.  The buffer size must be large enough
       for the maximum number of decimal digits that can fit in a seq_t. */
    char buf[128];
    size_t klen;
    seq_to_str(k, buf, &klen);

    /* Try every possible substring that does not start with '0'. */
    for (size_t i = 0; i < klen; ++i) {
        if (buf[i] == '0')
            continue;                    /* substring must not begin with 0 */

        for (size_t j = i; j < klen; ++j) {
            /* Compute the numeric value of the substring k[i..j]. */
            seq_t sub = 0;
            for (size_t p = i; p <= j; ++p)
                sub = sub * 10 + (buf[p] - '0');

            /* Double the substring. */
            seq_t doubled = sub * 2;

            /* Build the new number by replacing the substring with its double. */
            /* Buffer large enough for the resulting number. */
            char newbuf[256];
            size_t pos = 0;

            /* Prefix. */
            for (size_t p = 0; p < i; ++p)
                newbuf[pos++] = buf[p];

            /* Double.  Convert to string for easy appending. */
            char dblbuf[32];
            size_t dbl_len;
            seq_to_str(doubled, dblbuf, &dbl_len);
            for (size_t p = 0; p < dbl_len; ++p)
                newbuf[pos++] = dblbuf[p];

            /* Suffix. */
            for (size_t p = j + 1; p < klen; ++p)
                newbuf[pos++] = buf[p];

            newbuf[pos] = '\0';

            seq_t newnum = seq_from_str(newbuf);
            if (is_prime_big(newnum))
                return true;
        }
    }
    return false;
}

/* ---------- static storage for memoization ---------- */
static seq_t *terms = NULL;        /* array of found terms */
static size_t terms_cap = 0;        /* current capacity */
static size_t terms_cnt = 0;        /* current number of stored terms */
static seq_t next_k = 1;           /* smallest integer not yet tested */

/* Ensure there is room for one more term. */
static void ensure_terms_cap(void)
{
    if (terms_cnt >= terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 64;
        seq_t *tmp = mem_resize(seq_t, terms, new_cap);
        if (!tmp)
            abort();                  /* memory exhausted */
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* ---------- exported function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n = 0 */

    /* Generate terms until we have at least n of them. */
    while (terms_cnt < (size_t)n) {
        if (has_prime_substring(next_k)) {
            ensure_terms_cap();
            terms[terms_cnt++] = next_k;
        }
        ++next_k;
    }
    return terms[n - 1];
}
