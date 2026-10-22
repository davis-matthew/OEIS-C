/*
 * A349805  a(n) = A261138(n)/11.
 * https://oeis.org/A349805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A261138(n)/11
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A261138(n) is the palindrome formed by concatenating 1..(n-1), n, rev(n), rev(1..(n-1)). */
/* The function caches previously computed values. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n) {
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Static strings holding the concatenation 1..(k) and its reverse */
    static char *S = NULL;          /* concatenation 1..(k-1) */
    static size_t lenS = 0;
    static size_t capS = 0;
    static char *revS = NULL;       /* reverse of S (i.e., reverse of 1..(k-1)) */
    static size_t lenRevS = 0;
    static size_t capRevS = 0;

    /* Build cache up to requested n */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* Convert k to decimal string */
        char num_buf[32];
        int num_len = snprintf(num_buf, sizeof(num_buf), "%llu",
                               (unsigned long long)k);
        if (num_len <= 0) num_len = 0;

        /* Reverse k's decimal string */
        char rev_num_buf[32];
        for (int i = 0; i < num_len; ++i) {
            rev_num_buf[i] = num_buf[num_len - 1 - i];
        }
        rev_num_buf[num_len] = '\0';

        /* Build the palindrome A261138(k) */
        size_t pal_len = lenS + num_len + num_len + lenRevS;
        char *pal = (char *)malloc(pal_len + 1);
        if (!pal) abort(); /* allocation failed */

        size_t pos = 0;
        memcpy(pal + pos, S, lenS);
        pos += lenS;
        memcpy(pal + pos, num_buf, num_len);
        pos += num_len;
        memcpy(pal + pos, rev_num_buf, num_len);
        pos += num_len;
        memcpy(pal + pos, revS, lenRevS);
        pos += lenRevS;
        pal[pos] = '\0';

        /* Convert to seq_t and divide by 11 */
        seq_t val = seq_from_str(pal);
        seq_t result = val / 11;
        free(pal);

        /* Store in cache */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = result;

        /* Update S (append k) */
        if (lenS + num_len + 1 > capS) {
            size_t new_cap = capS ? capS * 2 : 64;
            while (new_cap < lenS + num_len + 1) new_cap *= 2;
            char *tmp = (char *)realloc(S, new_cap);
            if (!tmp) abort();
            S = tmp;
            capS = new_cap;
        }
        memcpy(S + lenS, num_buf, num_len);
        lenS += num_len;
        S[lenS] = '\0';

        /* Update revS (prepend rev(k)) */
        if (lenRevS + num_len + 1 > capRevS) {
            size_t new_cap = capRevS ? capRevS * 2 : 64;
            while (new_cap < lenRevS + num_len + 1) new_cap *= 2;
            char *tmp = (char *)realloc(revS, new_cap);
            if (!tmp) abort();
            revS = tmp;
            capRevS = new_cap;
        }
        /* Shift existing revS content forward */
        memmove(revS + num_len, revS, lenRevS);
        memcpy(revS, rev_num_buf, num_len);
        lenRevS += num_len;
        revS[lenRevS] = '\0';
    }

    return cache[n - 1];
}
