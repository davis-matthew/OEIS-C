/*
 * A279422  Numbers k such that k^2 has an odd number of digits and the middle
 *          digit is 2.
 * https://oeis.org/A279422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 26, u32 n <= 3078, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS sequence A279422: numbers k such that k^2 has an odd
 * number of decimal digits and the middle decimal digit of k^2 is 2.
 * This function builds the sequence by scan‑up searching for values
 * satisfying the requirement and memoises the results.  It works for
 * any unsigned integer type seq_t (8,16,32,64,128 bits or arbitrary
 * precision).  The algorithm is O(n) with a small constant factor.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Simple dynamic array for the found terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of cached terms */
static size_t cache_cap   = 0;   /* allocated capacity */

/* The next value of k to test.  It is held as seq_t so that the search
 * works correctly for any seq_t width. */
static seq_t next_k = 1;

/* Ensure that 'cache' can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t newcap = cache_cap < 64 ? 64 : cache_cap * 2;
    while (newcap < needed)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* unrecoverable memory error */
    cache = tmp;
    cache_cap = newcap;
}

/* Returns the n‑th term of A279422 (1‑based index). */
seq_t a(idx_t n)
{
    /* Generate terms until the cache contains at least n items. */
    while (cache_size < n)
    {
        /* Square k: use seq_t multiplication.  For small seq_t sizes
         * the product may overflow, but in such builds the sequence
         * is not meant to grow beyond that point. */
        seq_t sq = next_k * next_k;

        /* Extract decimal digits of 'sq' into an array 'digits'.
         * digits[0] holds the least‑significant digit. */
        uint8_t digits[128];
        size_t dlen = 0;
        seq_t temp = sq;
        while (temp > 0)
        {
            /* Using seq_t modulo and division with uint16_t constants. */
            seq_t rem = temp % (seq_t)10;
            temp = temp / (seq_t)10;
            digits[dlen++] = (uint8_t)rem;
        }
        if (dlen == 0)  /* handle the case k=0, not needed here */
        {
            digits[dlen++] = 0;
        }

        /* The number of digits must be odd and the middle digit (counted
         * from the most significant side) must be 2.  Because 'digits'
         * is stored reversed, the middle digit is digits[dlen/2]. */
        if ((dlen & 1) && digits[dlen / 2] == 2)
        {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = next_k;
        }

        /* Advance to the next candidate. */
        next_k++;
    }

    return cache[n - 1];
}
