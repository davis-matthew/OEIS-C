/*
 * A267431  Indices of Catalan numbers that are not of the form x^2 + y^2 +
 *          z^2 where x, y and z are integers.
 * https://oeis.org/A267431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A267431: indices n of Catalan numbers C_n that cannot be written
 * as x^2 + y^2 + z^2 (Legendre's three‑square theorem).  The test
 * for an odd part ≡ 7 (mod 8) and an even exponent of 2 is applied
 * to each Catalan number.  The function a(n) returns the n‑th such
 * index (1‑based). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static state */
static idx_t    last_checked = 0;   /* next n to process          */
static size_t   found_len    = 0;   /* number of indices stored   */
static size_t   found_cap    = 0;   /* capacity of the array      */
static idx_t  *found        = NULL; /* array of qualifying indices*/
static unsigned int odd_mod8 = 1;   /* odd part of C_{last_checked} modulo 8 */

/* ensure that we have at least 'need' indices stored */
static void
ensure_terms(size_t need)
{
    if (found_len >= need) return;

    /* allocate or grow the array as required */
    if (found_cap == 0) {
        found_cap = 32;
        found = (idx_t *)malloc(found_cap * sizeof(idx_t));
        if (!found) exit(1);
    }

    while (found_len < need) {
        idx_t n = last_checked;               /* current Catalan index */
        unsigned int pop = __builtin_popcountll((unsigned long long)n);
        unsigned int tz  = __builtin_ctzll((unsigned long long)(n + 1));
        unsigned int v2  = pop - tz;           /* exponent of 2 in C_n   */

        /* check the three‑square test: odd part ≡ 7 (mod 8) and v2 even */
        if ((odd_mod8 & 7U) == 7U && !(v2 & 1U)) {
            if (found_len == found_cap) {      /* grow array */
                found_cap *= 2;
                idx_t *tmp = (idx_t *)realloc(found, found_cap * sizeof(idx_t));
                if (!tmp) exit(1);
                found = tmp;
            }
            found[found_len++] = n;            /* store qualifying index */
        }

        /* advance odd_mod8 to C_{n+1} using the recurrence
         *   C_{n+1} = C_n * (2*(2n+1))/(n+2)   */
        uint64_t num = 2ULL * (2ULL * n + 1ULL); /* numerator */
        uint64_t den = n + 2ULL;                /* denominator */

        unsigned int tz_num = __builtin_ctzll(num);
        unsigned int tz_den = __builtin_ctzll(den);

        uint64_t odd_n = num >> tz_num;        /* odd part of numerator */
        uint64_t odd_d = den >> tz_den;        /* odd part of denominator */

        unsigned int new_odd = odd_mod8;
        new_odd = (new_odd * (odd_n & 7U)) & 7U;   /* multiply by odd_n mod 8 */
        new_odd = (new_odd * (odd_d & 7U)) & 7U;   /* divide by odd_d mod 8 */
        odd_mod8 = new_odd;

        last_checked++;                       /* move to next index */
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;                     /* sequence starts at offset 1 */
    ensure_terms((size_t)n);
    return (seq_t)found[n - 1];
}
