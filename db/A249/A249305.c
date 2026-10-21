/*
 * A249305  Primes p such that 2^p - p^2 is not squarefree.
 * https://oeis.org/A249305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the nth prime p such that 2^p – p^2 is not squarefree
 *            (equivalently p ≡ 2, 4, 89, or 115 (mod 147)). The sequence
 *            starts a(1)=2, a(2)=89, a(3)=149,
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

/* static cache of qualifying primes ----------------------------------- */
static seq_t *qual = NULL;      /* array storing the found primes */
static size_t qcnt = 0;         /* number of stored primes */
static size_t qcap = 0;         /* allocated capacity of 'qual' */
static uint64_t last_prime = 2; /* last prime that was examined */

/* append a prime to the cache ---------------------------------------- */
static void append_prime(uint64_t p)
{
    if (qcnt == qcap) {
        size_t new_cap = qcap ? qcap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(qual, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        qual = tmp;
        qcap = new_cap;
    }
    qual[qcnt++] = (seq_t)p;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialize cache on first call */
    if (qcnt == 0) {
        append_prime(2);          /* a(1) = 2 */
    }

    /* generate primes until we have at least n qualifying ones */
    while ((idx_t)qcnt < n) {
        uint64_t p = next_prime(last_prime + 1); /* next prime after last */
        last_prime = p;
        uint64_t r = p % 147;
        if (r == 2 || r == 4 || r == 89 || r == 115) {
            append_prime(p);
        }
    }

    /* index is 1‑based, array is 0‑based */
    return qual[n - 1];
}
