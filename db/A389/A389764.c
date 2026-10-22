/*
 * A389764  Even sub-perfect powers: odd perfect powers (not including 1)
 *          minus 1.
 * https://oeis.org/A389764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd perfect powers (excluding 1) minus 1
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates all odd perfect powers ≤ a bound
   sufficient to contain the first n terms.  It then removes duplicates,
   sorts them, and stores the values minus one in a static array.
   Subsequent calls to a(n) simply return the cached entry. */

#include <stdlib.h>

/* Comparator for seq_t values, usable with qsort */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* Static array to hold computed terms (minus one) */
#define MAX_TERMS 50000         /* safe for n ≤ 10 000 */
static seq_t  terms[MAX_TERMS];
static size_t terms_len = 0;

/* Static buffer to accumulate raw perfect powers before sorting */
#define MAX_POWS 200000
static seq_t  pow_buf[MAX_POWS];

/* Expand the cache so that it contains at least 'needed' terms */
static void ensure_cache(idx_t needed)
{
    if (needed <= terms_len) return;          /* already have enough */

    /* Initial bound: rough estimate */
    seq_t bound = (seq_t)4 * (seq_t)needed * (seq_t)needed;

    /* Recompute until we have at least 'needed' unique powers */
    while (1) {
        size_t pow_len = 0;

        /* Enumerate all odd bases up to sqrt(bound) */
        for (seq_t base = 3; base * base <= bound; base += 2) {
            seq_t power = base * base;        /* exponent 2 */
            while (power <= bound) {
                if (pow_len < MAX_POWS)
                    pow_buf[pow_len++] = power;
                /* Next exponent, stop if overflow would occur */
                if (power > bound / base) break;
                power *= base;
            }
        }

        /* Sort the collected powers */
        qsort(pow_buf, pow_len, sizeof(seq_t), cmp_seq);

        /* Unique‑filter and store minus one */
        size_t uniq = 0;
        for (size_t i = 0; i < pow_len; ++i) {
            if (i == 0 || pow_buf[i] != pow_buf[i - 1]) {
                if (uniq < MAX_TERMS)        /* safety check */
                    terms[uniq++] = pow_buf[i] - 1;
            }
        }

        terms_len = uniq;

        if (terms_len >= needed) break;       /* success */
        bound *= 2;                           /* enlarge bound and retry */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined, but safe */
    ensure_cache(n);
    return terms[n - 1];
}
