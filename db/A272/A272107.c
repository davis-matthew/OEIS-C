/*
 * A272107  Absolute primes in base 8: every permutation of digits in base 8
 *          is a prime (only the smallest representatives of the permutation
 *          classes are shown).
 * https://oeis.org/A272107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest decimal representatives of octal digit languages
   whose every permutation is prime.  a(1) is 2; a(n) is the nth such
   smallest number.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "../utils/is_prime_big.h"

/* helper: test whether all permutations of an octal digit multiset are prime.
   dig[] contains the digits sorted ascending; len is their count.  */
static bool all_perm_prime(const uint8_t *dig, size_t len)
{
    /* we iterate over all distinct permutations.  Since dig[] is sorted we
       can use a next_permutation routine that skips duplicates.  */
    uint8_t *p = (uint8_t *)malloc(len * sizeof(uint8_t));
    if (!p) exit(1);
    memcpy(p, dig, len * sizeof(uint8_t));

    bool first = true;
    for (;;)
    {
        /* build the decimal value of this permutation */
        seq_t val = 0;
        for (size_t i = 0; i < len; ++i)
            val = val * 8 + p[i];

        if (!is_prime_big(val))
        {
            free(p);
            return false;
        }

        /* next permutation */
        size_t i = len - 1;
        while (i > 0 && p[i - 1] >= p[i]) --i;
        if (i == 0) break;
        size_t j = len;
        while (p[--j] <= p[i - 1]) ;
        uint8_t tmp = p[i - 1]; p[i - 1] = p[j]; p[j] = tmp;

        /* reverse suffix */
        for (size_t l = i, r = len - 1; l < r; ++l, --r)
        {
            tmp = p[l]; p[l] = p[r]; p[r] = tmp;
        }
    }

    free(p);
    return true;
}

/* helper: test whether a decimal number is an absolute prime in base 8 */
static bool is_abs_prime_oct(seq_t n)
{
    if (!is_prime_big(n))
        return false;

    /* extract octal digits (least significant first) into an array */
    uint8_t digits[64];
    size_t dlen = 0;
    seq_t m = n;
    do
    {
        digits[dlen++] = (uint8_t)(m % 8);
        m /= 8;
    } while (m > 0);

    /* create a sorted copy for permutation checks */
    uint8_t sorted[64];
    memcpy(sorted, digits, dlen);
    /* sort ascending (simple insertion sort for small arrays) */
    for (size_t i = 1; i < dlen; ++i)
    {
        uint8_t key = sorted[i];
        size_t j = i;
        while (j > 0 && sorted[j - 1] > key) { sorted[j] = sorted[j - 1]; --j; }
        sorted[j] = key;
    }

    /* all permutations of the sorted digits are prime? */
    return all_perm_prime(sorted, dlen);
}

/* cache for already seen digit‐multisets */
static size_t key_cnt = 0, key_cap = 0;
static char **keys = NULL;

/* linear search: true if key exists */
static bool key_exists(const char *key)
{
    for (size_t i = 0; i < key_cnt; ++i)
        if (strcmp(keys[i], key) == 0)
            return true;
    return false;
}

/* insert a new key.  caller guarantees it does not already exist. */
static void add_key(const char *key)
{
    if (key_cnt == key_cap)
    {
        size_t new_cap = key_cap ? key_cap * 2 : 8;
        char **tmp = (char **)realloc(keys, new_cap * sizeof(*keys));
        if (!tmp) exit(1);
        keys = tmp;
        key_cap = new_cap;
    }
    keys[key_cnt] = strdup(key);
    if (!keys[key_cnt]) exit(1);
    ++key_cnt;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static cache of sequence values */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static seq_t next_candidate = 2;   /* next decimal to test */

    while (cache_len < (size_t)n)
    {
        /* compute canonical key of the digit multiset of next_candidate */
        /* extract digits */
        uint8_t dig[64];
        size_t dlen = 0;
        seq_t tmp = next_candidate;
        do {
            dig[dlen++] = (uint8_t)(tmp % 8);
            tmp /= 8;
        } while (tmp > 0);

        /* sort ascending to form key */
        uint8_t sorted[64];
        memcpy(sorted, dig, dlen);
        for (size_t i = 1; i < dlen; ++i)
        {
            uint8_t key = sorted[i];
            size_t j = i;
            while (j > 0 && sorted[j - 1] > key) { sorted[j] = sorted[j - 1]; --j; }
            sorted[j] = key;
        }

        /* build key string */
        char keybuf[64];
        for (size_t i = 0; i < dlen; ++i)
            keybuf[i] = '0' + sorted[i];
        keybuf[dlen] = '\0';

        if (!key_exists(keybuf))
        {
            /* test permutations */
            if (is_abs_prime_oct(next_candidate))
            {
                add_key(keybuf);

                /* grow cache */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
                    if (!tmp) exit(1);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = next_candidate;
            }
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
