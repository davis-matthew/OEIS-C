/*
 * A359912  Numbers whose prime indices do not have integer median.
 * https://oeis.org/A359912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, sort_indices
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A359912
   Numbers whose prime indices do not have integer median.
   Computed by a brute‑force search of successive integers.
   An integer n is accepted iff the multiset of indices of primes
   dividing n has even cardinality and the sum of its two middle indices
   is odd (which makes the median non‑integer).  The integer 1 is
   accepted by definition. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* helper prototypes from the build environment */
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */
#include "../utils/factor_u64.h"          /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/sort_indices.h"

static uint64_t *prime_list = NULL;   /* prime_list[i] = (i+1)-th prime (1‑based index = i+1) */
static size_t     prime_cnt  = 0;

/* initialise prime list once */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;
    /* 2 000 000 is far larger than any prime factor needed for
       the first ten thousand terms. */
    idx_t limit = 2000000;
    build_primes_upto(limit, &prime_list, &prime_cnt);
}

/* binary search for the 1‑based index of prime p in prime_list */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo points to the element equal to p */
    return lo + 1; /* convert to 1‑based index */
}

/* test whether the integer x satisfies the condition */
static bool good_number(uint64_t x)
{
    if (x == 1)                 /* 1 has the empty multiset */
        return true;

    /* factorisation */
    uint64_t pf[64];
    uint64_t ex[64];
    size_t pcnt = 0;
    factor_u64(x, pf, ex, &pcnt);

    /* build the multiset of prime indices (with multiplicity) */
    size_t idxs[64];
    size_t cnt = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p   = pf[i];
        size_t  idx  = prime_index(p);
        for (uint64_t k = 0; k < ex[i]; ++k) {
            if (cnt < sizeof(idxs) / sizeof(idxs[0]))
                idxs[cnt++] = idx;
        }
    }

    /* odd length ⇒ integer median; exclude */
    if ((cnt & 1) != 0)
        return false;

    /* median non‑integer ⇔ sum of the two middle indices is odd */
    sort_indices(idxs, cnt);
    size_t mid1 = idxs[cnt / 2 - 1];
    size_t mid2 = idxs[cnt / 2];
    return ((mid1 + mid2) & 1U) == 1;
}

/* ------------------------------------------------------------------
   Public sequence function.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* safety: OEIS offset starts at 1 */

    init_primes();

    idx_t   found = 0;        /* count of numbers satisfying the property so far */
    uint64_t num  = 1;        /* current candidate integer */

    while (true) {
        if (good_number(num)) {
            ++found;
            if (found == n)
                return (seq_t)num;
        }
        ++num;
    }
}
