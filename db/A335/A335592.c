/*
 * A335592  a(n) is the determinant of the 2 X 2 matrix whose entries (when
 *          read by rows) are the n-th primes ending in 1, 3, 7, 9
 *          respectively.
 * https://oeis.org/A335592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A335592: determinant of the 2×2 matrix whose entries
   (when read by rows) are the n‑th primes ending in 1, 3, 7, 9. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ensure_capacity.h"

static idx_t  cur_limit      = 0;
static uint64_t *all_primes   = NULL;
static size_t   all_prime_cnt = 0;

static size_t cap1 = 0, cnt1 = 0, cap3 = 0, cnt3 = 0;
static size_t cap7 = 0, cnt7 = 0, cap9 = 0, cnt9 = 0;
static uint64_t *pr1 = NULL;
static uint64_t *pr3 = NULL;
static uint64_t *pr7 = NULL;
static uint64_t *pr9 = NULL;

static idx_t max_needed = 0;

/* Ensure that we have at least n primes for each residue 1,3,7,9
   ending modulo 10.  The sieve limit is doubled as necessary. */
static void ensure_primes(idx_t n)
{
    if (max_needed >= n) return;
    max_needed = n;
    if (cur_limit == 0) cur_limit = 100000;   /* initial estimate */

    /* Keep extending until we get enough primes. */
    while (1) {
        /* free previously generated list of all primes */
        if (all_primes) free(all_primes);

        /* generate primes up to cur_limit */
        build_primes_upto(cur_limit, &all_primes, &all_prime_cnt);

        /* reset counts */
        cnt1 = cnt3 = cnt7 = cnt9 = 0;

        for (size_t i = 0; i < all_prime_cnt; ++i) {
            uint64_t p = all_primes[i];
            switch ((uint32_t)(p % 10)) {
                case 1:
                    if (cnt1 < n) {
                        ensure_capacity(cnt1 + 1, &cap1, (void **)&pr1, sizeof(uint64_t));
                        pr1[cnt1++] = p;
                    }
                    break;
                case 3:
                    if (cnt3 < n) {
                        ensure_capacity(cnt3 + 1, &cap3, (void **)&pr3, sizeof(uint64_t));
                        pr3[cnt3++] = p;
                    }
                    break;
                case 7:
                    if (cnt7 < n) {
                        ensure_capacity(cnt7 + 1, &cap7, (void **)&pr7, sizeof(uint64_t));
                        pr7[cnt7++] = p;
                    }
                    break;
                case 9:
                    if (cnt9 < n) {
                        ensure_capacity(cnt9 + 1, &cap9, (void **)&pr9, sizeof(uint64_t));
                        pr9[cnt9++] = p;
                    }
                    break;
            }
            if (cnt1 >= n && cnt3 >= n && cnt7 >= n && cnt9 >= n)
                break;
        }

        /* if we have enough primes of all types, break */
        if (cnt1 >= n && cnt3 >= n && cnt7 >= n && cnt9 >= n)
            break;

        /* otherwise increase the sieve limit and try again */
        cur_limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* index 0 is out of spec, safe default */
    ensure_primes(n);

    seq_t p1 = (seq_t)pr1[n - 1];
    seq_t p2 = (seq_t)pr3[n - 1];
    seq_t p3 = (seq_t)pr7[n - 1];
    seq_t p4 = (seq_t)pr9[n - 1];

    return p1 * p4 - p2 * p3;
}
