/*
 * A366826  Composite numbers whose proper substrings (of their decimal
 *          expansions) are all primes.
 * https://oeis.org/A366826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = the n‑th composite number whose proper substrings of its
 *            decimal expansion are all primes (OEIS A366826). The
 *            implementation precomputes all such numbers up to 999 once and
 *            answers queries by indexing into the resulting array
 * Range:     u8 n <= 17, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

static seq_t *valid = NULL;      /* array of qualifying numbers */
static size_t valid_cnt = 0;     /* number of stored elements */

/* Ensure the table of valid numbers is filled (once). */
static void build_table(void)
{
    if (valid != NULL) return;   /* already built */

    size_t cap = 16;
    valid = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!valid) abort();

    /* scan all numbers from 4 to 999 */
    for (idx_t num = 4; num <= 999; ++num) {
        /* single–digit composite numbers */
        if (num < 10) {
            if (!is_prime_small((seq_t)num)) {
                if (valid_cnt == cap) {
                    cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(valid, cap * sizeof(seq_t));
                    if (!tmp) abort();
                    valid = tmp;
                }
                valid[valid_cnt++] = (seq_t)num;
            }
            continue;
        }

        /* get decimal digits in normal order */
        uint8_t rev[10], digs[10];
        size_t len = 0;
        idx_t t = num;
        while (t > 0) {
            rev[len++] = t % 10;
            t /= 10;
        }
        for (size_t i = 0; i < len; ++i)
            digs[i] = rev[len - 1 - i];

        /* test all proper substrings */
        bool ok = true;
        for (size_t i = 0; i < len && ok; ++i) {
            seq_t sub = 0;
            for (size_t j = i; j < len; ++j) {
                sub = sub * 10 + digs[j];
                /* full number is a proper substring? skip it */
                if ((j - i + 1) == len) break;
                if (!is_prime_small(sub)) {
                    ok = false;
                    break;
                }
            }
        }

        if (ok && !is_prime_small((seq_t)num)) {
            if (valid_cnt == cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(valid, cap * sizeof(seq_t));
                if (!tmp) abort();
                valid = tmp;
            }
            valid[valid_cnt++] = (seq_t)num;
        }
    }
}

seq_t a(idx_t n)
{
    build_table();            /* populate table on first call */
    if (n == 0) return (seq_t)0;
    if ((idx_t)valid_cnt >= n) return valid[n - 1];
    return (seq_t)0;
}
