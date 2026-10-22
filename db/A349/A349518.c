/*
 * A349518  a(n)=x is the least prime with pi(x,4,3) - pi(x,4,1) = n-1 where
 *          pi(x,4,k) is the number of primes 4*j + k <= x.
 * https://oeis.org/A349518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A349518: a(n) = least prime p such that pi(p,4,3) - pi(p,4,1) = n-1
 * (pi(x,4,k) counts primes ≤ x with remainder k mod 4)
 * Implementation uses memoised sieve on the fly.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *diff_prime = NULL;      /* diff_prime[diff] = smallest prime with that diff */
static size_t diff_cap   = 0;          /* array capacity */
static uint64_t current_prime = 0;     /* last prime processed (uint64_t) */
static int64_t  current_diff  = 0;     /* current value of pi(...,3)-pi(...,1) */
static bool     initialized   = false;

static void ensure_capacity(size_t needed)
{
    if (needed < diff_cap)
        return;
    size_t new_cap = diff_cap > 0 ? diff_cap : 8;
    while (new_cap <= needed) new_cap <<= 1;
    diff_prime = (seq_t*)realloc(diff_prime, new_cap * sizeof(seq_t));
    /* Initialize the newly allocated part to 0 (i.e., zero the integer) */
    memset(diff_prime + diff_cap, 0, (new_cap - diff_cap) * sizeof(seq_t));
    diff_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* 1‑based index handling */
    if (n == 0) return 0; /* undefined, but avoid UB */

    /* Initialise once */
    if (!initialized) {
        diff_cap = 8;
        diff_prime = (seq_t*)calloc(diff_cap, sizeof(seq_t));
        diff_prime[0] = (seq_t)2ULL;      /* diff 0 occurs at prime 2 */
        current_prime = 2ULL;
        current_diff  = 0;
        initialized   = true;
    }

    size_t target = (size_t)(n - 1);          /* required diff */
    ensure_capacity(target);                  /* ensure array large enough */

    /* If already known, return it */
    if (diff_prime[target] != 0)
        return diff_prime[target];

    /* Generate primes until we cover target diff */
    while (diff_prime[target] == 0) {
        /* Find next prime > current_prime */
        uint64_t cand = current_prime + 1;
        if (cand <= 2) cand = 3;
        if ((cand & 1ULL) == 0) cand++;     /* make odd */

        /* Simple primality test using provided utility */
        while (!is_prime((idx_t)cand))
            cand += 2;
        current_prime = cand;

        /* Update difference */
        if ((cand & 3ULL) == 3)          /* prime ≡ 3 (mod 4) */
            current_diff++;
        else if ((cand & 3ULL) == 1)     /* prime ≡ 1 (mod 4) */
            current_diff--;

        /* Record prime for this diff if non‑negative */
        if (current_diff >= 0) {
            size_t diff_idx = (size_t)current_diff;
            ensure_capacity(diff_idx);
            if (diff_prime[diff_idx] == 0)
                diff_prime[diff_idx] = (seq_t)current_prime;
        }
    }
    return diff_prime[target];
}
