/*
 * A267371  Limiting sequence formed by starting with 01 and iteratively
 *          concatenating existing string with the shortest prefix that
 *          appears only once.
 * https://oeis.org/A267371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Start with the binary string "01".  Repeatedly append the
 * shortest prefix that occurs only once in the current string.
 * a(n) is the n‑th bit of the resulting string (1‑based indexing). */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static uint8_t *S = NULL;  /* current sequence of 0/1 bytes          */
static size_t len = 0;     /* number of elements in S                */
static size_t cap = 0;     /* allocated capacity of S                */

/* Ensure that S has room for at least 'need' elements.  */
static void ensure_cap(size_t need)
{
    if (need <= cap)
        return;
    size_t newcap = cap ? cap << 1 : 64;
    while (newcap < need)
        newcap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(S, newcap * sizeof(uint8_t));
    if (!tmp)
        abort();          /* memory allocation failed */
    S = tmp;
    cap = newcap;
}

seq_t a(idx_t n)
{
    /* Convert to a size_t for array indexing.  */
    size_t target = (size_t)n;
    if (target == 0)
        return (seq_t)0;   /* undefined for n=0, but harmless */

    /* Initialise the base string "01" once. */
    if (len == 0) {
        ensure_cap(2);
        S[0] = 0;
        S[1] = 1;
        len = 2;
    }

    /* Grow the sequence until it contains the requested element. */
    while (len < target) {
        /* Find the shortest prefix that appears only once. */
        size_t l_found = 0;
        for (size_t l = 1; l <= len; ++l) {
            size_t count = 0;
            for (size_t j = 0; j + l <= len; ++j) {
                size_t k;
                for (k = 0; k < l; ++k)
                    if (S[k] != S[j + k])
                        break;
                if (k == l) {           /* substring matched */
                    ++count;
                    if (count > 1)
                        break;
                }
            }
            if (count == 1) {
                l_found = l;
                break;
            }
        }

        /* Append the found prefix to the sequence. */
        ensure_cap(len + l_found);
        memcpy(&S[len], S, l_found * sizeof(uint8_t));
        len += l_found;
    }

    return (seq_t)S[target - 1];
}
