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
 * Range:     u8 n <= 27, u16 n <= 1728, u32 n <= 2575, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_from_str, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- helpers ---------- */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/seq_to_str.h"       /* seq_to_str(seq_t, char*, size_t*) */
#include "../utils/seq_from_str.h"     /* seq_from_str(const char*) -> seq_t */
#include "../utils/is_prime_big.h"     /* is_prime_big(seq_t) -> bool */

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
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
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
