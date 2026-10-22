/*
 * A386094  Primes having only {1, 2, 4, 9} as digits.
 * https://oeis.org/A386094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   this function enumerates primes whose decimal digits belong to *
 *            the set {1, 2, 4, 9}. It keeps a static cache of the sequence
 *            and * dynamically generates candidates in lexicographic (hence
 *            numeric) * order, testing each for primality with is_prime_big.
 *            If the requested * index is already in the...
 * Range:     u8 n <= 11, u16 n <= 147, u32 n <= 48, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq_array.h"

/* Return the nth term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    /* Cache of found primes. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of cached terms           */
    static size_t cache_cap = 0;   /* allocated capacity of cache      */

    /* Lexicographic counter over the allowed digits. */
    enum { MAX_LEN = 32 };                 /* enough for 128‑bit numbers      */
    static unsigned char idx_arr[MAX_LEN] = {0};
    static int cur_len = 1;                /* current number of digits        */

    /* The few digits permitted in the decimal representation. */
    static const seq_t DIGITS[4] = {(seq_t)1, (seq_t)2, (seq_t)4, (seq_t)9};

    /* If the requested term is already cached, return it. */
    if ((idx_t)n <= cache_len)
        return cache[n - 1];

    /* Number of terms we need to generate. */
    const size_t target = (size_t)n;

    /* Generate candidates until we have enough primes. */
    while (cache_len < target) {
        /* Build the current candidate from idx_arr. */
        seq_t val = 0;
        for (int i = 0; i < cur_len; ++i)
            val = val * 10 + DIGITS[idx_arr[i]];

        /* Test for primality and store if prime. */
        if (is_prime_big(val)) {
            if (cache_len == cache_cap)
                grow_seq_array(&cache, &cache_cap, cache_cap ? cache_cap * 2 : 64);
            cache[cache_len++] = val;
        }

        /* Advance the base‑4 counter. */
        int pos = cur_len - 1;
        while (pos >= 0) {
            ++idx_arr[pos];
            if (idx_arr[pos] < 4)
                break;
            idx_arr[pos] = 0;
            --pos;
        }

        /* If all numbers of this length were exhausted, go to the next
         * length and reset the counter. */
        if (pos < 0) {
            ++cur_len;
            for (int i = 0; i < cur_len; ++i)
                idx_arr[i] = 0;
        }
    }

    /* The desired term is now cached. */
    return cache[n - 1];
}
