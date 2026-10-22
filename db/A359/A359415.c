/*
 * A359415  Numbers k such that phi(k) is a 5-smooth number where phi is the
 *          Euler totient function.
 * https://oeis.org/A359415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer k such that φ(k) is 5‑smooth
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   is_5smooth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_5smooth.h"

/* Arrays used by the linear sieve */
static uint64_t *phi_arr     = NULL;   /* phi values */
static unsigned char *isComp = NULL;   /* composite flags */
static uint32_t *primes       = NULL;   /* list of primes */
static size_t primes_cnt      = 0;      /* number of primes found */
static size_t sieve_limit     = 0;      /* current size of arrays */

/* Ensure that the arrays can hold up to 'need' elements */
static void ensure_capacity(size_t need, size_t *cap, void **ptr, size_t elsize)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 64 : *cap << 1;
        while (newcap < need) newcap <<= 1;
        void *tmp = realloc(*ptr, newcap * elsize);
        if (!tmp) exit(1);                 /* out of memory */
        *ptr = tmp;
        *cap = newcap;
    }
}

/* Extend the sieve up to 'limit' (inclusive) */
static void extend_sieve(size_t limit)
{
    if (limit <= sieve_limit) return;

    /* Allocate (or enlarge) the arrays */
    phi_arr   = (uint64_t*)realloc(phi_arr,   (limit + 1) * sizeof(uint64_t));
    isComp    = (unsigned char*)realloc(isComp,    (limit + 1) * sizeof(unsigned char));
    primes    = (uint32_t*)realloc(primes, (limit + 1) * sizeof(uint32_t));
    if (!phi_arr || !isComp || !primes) exit(1);

    /* Initialise new portion */
    for (size_t i = sieve_limit + 1; i <= limit; ++i) {
        phi_arr[i] = 0;
        isComp[i]  = 0;
    }
    if (sieve_limit < 1) {
        phi_arr[1] = 1;      /* φ(1) = 1 */
        isComp[1] = 0;
    }

    /* Linear sieve */
    size_t start = (sieve_limit + 1 > 2) ? sieve_limit + 1 : 2;
    for (size_t i = start; i <= limit; ++i) {
        if (!isComp[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            phi_arr[i] = i - 1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint64_t p = primes[j];
            uint64_t ip = i * p;
            if (ip > limit) break;
            isComp[ip] = 1;
            if (i % p == 0) {
                phi_arr[ip] = phi_arr[i] * p;
                break;
            } else {
                phi_arr[ip] = phi_arr[i] * (p - 1);
            }
        }
    }

    sieve_limit = limit;
}

/* Storage for the sequence values: the k’s themselves */
static seq_t *valid_seq   = NULL;
static size_t valid_len   = 0;
static size_t valid_cap   = 0;
static idx_t last_checked = 0;            /* largest k processed */

/* Return the n‑th term (1‑based) of the sequence */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Grow the list until we have at least n terms */
    while (valid_len < (size_t)n) {
        /* Choose a chunk size that grows quickly */
        size_t to = last_checked + 10000;
        if (to <= (size_t)last_checked) to = last_checked + 1;
        if (to < 1000) to = 1000;

        extend_sieve(to);

        /* Inspect new numbers */
        for (idx_t k = last_checked + 1; k <= (idx_t)to; ++k) {
            if (is_5smooth((uint64_t)phi_arr[(size_t)k])) {
                ensure_capacity(valid_len + 1, &valid_cap, (void**)&valid_seq,
                                sizeof(seq_t));
                valid_seq[valid_len++] = (seq_t)k;
            }
        }
        last_checked = (idx_t)to;
    }

    return valid_seq[n - 1];
}
