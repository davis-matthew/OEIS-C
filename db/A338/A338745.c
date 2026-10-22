/*
 * A338745  When a(n) is even, a(n) is the number of prime digits present so
 *          far in the sequence, a(n) included.
 * https://oeis.org/A338745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A338745.  The sequence is generated
   incrementally as follows:
     • Keep track of the current total (C) of prime digits (2,3,5,7)
       seen so far.
     • At each step the next term is either
           (a) the smallest unused even integer e that satisfies
               e - d(e) == C   (d(e) = number of prime digits in e),
           or
           (b) the smallest unused odd integer.
       If (a) exists it is chosen; otherwise (b) is chosen.
     • The chosen value is marked as used, appended to the cache,
       and C is updated by adding d(value) before the next step.
   The function is memoised; after its first use it can answer any
   subsequent call to a(idx_t n) in O(1).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* count prime digits {2,3,5,7} in a sequence value */
static inline unsigned char prime_digit_count(seq_t x)
{
    unsigned char cnt = 0;
    while (x != 0) {
        unsigned digit = (unsigned)(x % 10);
        if (digit == 2 || digit == 3 || digit == 5 || digit == 7)
            ++cnt;
        x /= 10;
    }
    return cnt;
}

/* OEIS sequence A338745 */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache          = NULL;
    static size_t cache_size      = 0, cache_cap = 0;
    static seq_t prime_digit_cnt  = 0;          /* cumulative count */
    static size_t next_odd        = 3;          /* smallest unused odd >1 */
    static unsigned char *used    = NULL;      /* used[0..used_cap-1] */
    static size_t used_cap        = 0;

    /* initialise static data on the first call */
    if (!cache) {
        /* cache for a(1..) */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(1) = 1 */
        cache_size = 1;
        prime_digit_cnt = prime_digit_count((seq_t)1);

        /* used array up to an initial limit */
        used_cap = 128;
        used = (unsigned char *)calloc(used_cap, 1);
        used[1] = 1;                  /* 1 is used */
    }

    /* generate terms until we have at least n entries */
    while (cache_size < n) {
        seq_t term = 0;

        /* 1. try to find an even candidate */
        seq_t e = 0;
        /* start search at max(2, prime_digit_cnt) – the smallest even
           number that could satisfy e - d(e) == prime_digit_cnt */
        e = prime_digit_cnt;
        if (e < 2) e = 2;
        if ((e & 1) != 0) e++;              /* make even */

        /* search upward by even steps */
        for (;; e += 2) {
            size_t idx = (size_t)e;
            if (idx >= used_cap) {
                /* grow used array */
                size_t old_cap = used_cap;
                while (idx >= used_cap) used_cap <<= 1;
                unsigned char *tmp = (unsigned char *)realloc(used, used_cap);
                if (!tmp) { free(cache); free(used); return 0; }
                used = tmp;
                memset(used + old_cap, 0, used_cap - old_cap);
            }
            if (used[idx])          /* already used */
                continue;
            if (e - prime_digit_count(e) == prime_digit_cnt) {
                term = e;
                break;
            }
            /* some bounds to avoid infinite loop: the candidate must
               not be far larger than prime_digit_cnt + 50 – this is
               safe for the intended range (n ≤ 10000).  */
            if (e > prime_digit_cnt + 1024) /* fall back to odd */
                break;
        }

        /* 2. if no even candidate, pick smallest unused odd */
        if (term == 0) {
            /* advance until we find an unused odd */
            while (next_odd < used_cap && used[(size_t)next_odd])
                next_odd += 2;
            /* grow used if necessary */
            if (next_odd >= used_cap) {
                size_t old_cap = used_cap;
                while (next_odd >= used_cap) used_cap <<= 1;
                unsigned char *tmp = (unsigned char *)realloc(used, used_cap);
                if (!tmp) { free(cache); free(used); return 0; }
                used = tmp;
                memset(used + old_cap, 0, used_cap - old_cap);
            }
            term = next_odd;
            used[(size_t)next_odd] = 1;
            next_odd += 2;
        } else {
            used[(size_t)term] = 1;
        }

        /* store the term and update the prime‑digit counter */
        if (cache_size >= cache_cap) {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) { free(cache); free(used); return 0; }
            cache = tmp;
        }
        cache[cache_size++] = term;
        prime_digit_cnt += prime_digit_count(term);
    }

    return cache[n - 1];
}
