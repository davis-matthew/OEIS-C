/*
 * A303502  Integers k such that the digits of k together with a single
 *          supplementary digit can be reordered to form a base-10 palindrome.
 * https://oeis.org/A303502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer k such that the multiset of
 *            the decimal digits of k together with one extra digit can be
 *            permuted into a base‑10 palindrome. The integer k is tested by
 *            computing the number of digits with odd frequency and applying
 *            the parity condition derived above
 * Range:     u8 n <= 147, u16 n <= 4816, u32 n <= 4816, u64 n <= 4816,
 *            u128 n <= 4816, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* test whether integer k satisfies the palindrome‑with‑one‑digit condition */
static bool good_candidate(idx_t k)
{
    unsigned char cnt[10] = {0};
    idx_t tmp = k;
    int len = 0;
    while (tmp > 0) {
        cnt[tmp % 10]++;   /* tmp%10 is the least significant decimal digit */
        tmp /= 10;
        ++len;
    }

    /* count how many digits appear an odd number of times */
    int odd = 0;
    for (int d = 0; d < 10; ++d)
        if (cnt[d] & 1)
            ++odd;

    if ((len & 1) == 0) {                 /* len even -> L even */
        /* After adding one digit we need exactly one odd count. */
        return (odd == 0 || odd == 2);
    } else {                               /* len odd -> L odd */
        /* After adding one digit we need zero odd counts. */
        return (odd == 1);
    }
}

/* dynamic storage for memoized sequence values */
static seq_t *cached_seq = NULL;
static size_t cached_cnt = 0;
static size_t cached_cap = 0;
static idx_t next_search = 1;  /* next integer to test for the sequence */

/* make sure the cache can hold at least 'need' entries */
static void grow_cache(size_t need)
{
    if (need <= cached_cap)
        return;
    size_t new_cap = cached_cap ? (cached_cap << 1) : 16;
    while (new_cap < need)
        new_cap <<= 1;
    cached_seq = (seq_t *)realloc(cached_seq, new_cap * sizeof(seq_t));
    cached_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined for n = 0, safety return */

    /* generate sequence elements until we have n terms */
    while (cached_cnt < (size_t)n) {
        if (good_candidate(next_search)) {
            grow_cache(cached_cnt + 1);
            cached_seq[cached_cnt++] = (seq_t)next_search;
        }
        ++next_search;
    }
    return cached_seq[(size_t)n - 1];
}
