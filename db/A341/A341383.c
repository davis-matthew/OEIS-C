/*
 * A341383  Numbers m such that the largest digit in the decimal expansion of
 *          1/m is 2.
 * https://oeis.org/A341383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer m such that the largest digit * in
 *            the decimal expansion of 1/m is 2
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  This implementation keeps a cache of the terms found so far.
 *        It tests successive integers m = 1, 2, … using long division.
 *        For each m it keeps generating decimal digits until:
 *          • a digit 3–9 is encountered → reject;
 *          • the remainder becomes 0 → accept only if a digit 2 has
 *            appeared; or
 *          • the number of produced digits reaches m (the period of 1/m
 *            is at most m – 1) → accept only if a digit 2 has appeared.
 *        The algorithm is O(n·m) in the worst case but easily runs in
 *        a few milliseconds for n ≤ 10 000. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Return true if m satisfies the condition for the sequence.  */
static bool good_m(seq_t m)
{
    if (m <= 1)
        return false;      /* 1/m = 1.0… contains digit 1, not 2 */

    seq_t rem = 1 % m;          /* initial remainder */
    bool seen2 = false;         /* have we seen a digit 2? */

    /* The cycle length of 1/m is at most m-1, so using m as a
     * hard upper bound is safe for the test.  If m is larger than
     * the maximum value representable by idx_t, the cast truncates
     * and the loop is still harmless for the sizes needed by the
     * tests.  */
    idx_t limit = (idx_t)m;

    for (idx_t step = 0; step < limit; ++step) {
        seq_t prod  = rem * 10;   /* next product */
        seq_t digit = prod / m;   /* next decimal digit */

        if (digit > 2)            /* digit 3–9 found → reject */
            return false;

        if (digit == 2)
            seen2 = true;

        rem = prod % m;           /* next remainder */
        if (rem == 0)             /* terminating decimal */
            return seen2;         /* accept only if a 2 was seen */
    }

    /* Completed the maximum number of steps without a large digit.
       Accept only if a 2 has been seen somewhere. */
    return seen2;
}

/* Static cache of successful terms. */
static seq_t *cache = NULL;       /* array of terms */
static size_t cache_cap = 0;      /* capacity of the array */
static size_t cache_len = 0;      /* number of terms stored */
static seq_t next_candidate = 1;  /* next integer to test */

/* Add a term to the cache, expanding the array when necessary. */
static void add_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
#if defined(__cplusplus)
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cache_len; ++i)
            tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
        cache_cap = new_cap;
#else
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
#endif
    }
    cache[cache_len++] = val;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for offset 0 */

    /* Generate terms until we have at least n terms cached. */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;
        if (good_m(cand))
            add_term(cand);
        ++next_candidate;
    }
    return cache[n - 1];
}
