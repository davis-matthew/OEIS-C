/*
 * A395103  Numbers with exactly k prime factors, counting repeats, with k>1,
 *          that are formed by concatenating k primes.
 * https://oeis.org/A395103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A395103 = numbers with exactly k>1 prime factors (counting *
 *            multiplicity) that can be written as the concatenation of k
 *            primes. * This implementation searches integers incrementally
 * Range:     u8 n <= 19, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   count_prime_factors, digit_len, is_prime_big, seq_from_str,
 *            seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/count_prime_factors.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_from_str.h"
#include "../utils/seq_to_str.h"
#include "../utils/digit_len.h"

/* ------------------------------------------------------------------ */
/*  Helper: recursive split into `parts_left` primes.                  */
/* ------------------------------------------------------------------ */
static bool dfs_split(const char *s, size_t pos, int parts_left, size_t total_len)
{
    if (parts_left == 0)
        return pos == total_len;
    if (pos == total_len)
        return false;
    if (parts_left > (int)(total_len - pos))
        return false;                           /* not enough digits left */

    if (s[pos] == '0')
        return false;                           /* leading zero – invalid prime */

    for (size_t end = pos + 1; end <= total_len; ++end) {
        /* Temporary buffer for the substring; 512 is ample for our search. */
        static char subbuf[512];
        size_t sublen = end - pos;
        if (sublen >= sizeof(subbuf))
            continue;                           /* substring too long, skip */

        memcpy(subbuf, s + pos, sublen);
        subbuf[sublen] = '\0';

        seq_t part = seq_from_str(subbuf);
        if (is_prime_big(part) && dfs_split(s, end, parts_left - 1, total_len))
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Test whether `x` can be split into `k` prime parts.                  */
/* ------------------------------------------------------------------ */
static bool can_split_into_k_primes(seq_t x, idx_t k)
{
    if (k < 2)
        return false;

    size_t len = digit_len(x);
    if (len < (size_t)k)
        return false;                           /* not enough digits */

    /* Allocate enough space for decimal representation. */
    char *s = (char *)malloc(len + 1);
    if (!s)
        abort();
    seq_to_str(x, s, &len);                      /* len is updated but unchanged */

    bool ok = dfs_split(s, 0, (int)k, len);

    free(s);
    return ok;
}

/* ------------------------------------------------------------------ */
/*  Cache all found terms.                                             */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;      /* array of cached terms */
static size_t cache_capacity = 0;
static size_t cache_count    = 0;
static seq_t current_value = 21;  /* first candidate is 22 */

/* ------------------------------------------------------------------ */
/*  Main sequence function.                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Fill cache until we have at least `n` terms. */
    while (cache_count < n) {
        ++current_value;                       /* next integer to test     */

        seq_t k_seq = count_prime_factors(current_value);
        if (k_seq < (seq_t)2)
            continue;                           /* skip numbers with ≤1 prime factor */

        idx_t k = (idx_t)k_seq;
        if (!can_split_into_k_primes(current_value, k))
            continue;                           /* split condition failed    */

        /* Ensure capacity. */
        if (cache_count >= cache_capacity) {
            size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            cache       = tmp;
            cache_capacity = new_cap;
        }

        cache[cache_count++] = current_value;
    }

    return cache[n - 1];
}
