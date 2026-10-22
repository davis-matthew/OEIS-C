/*
 * A348664  Numbers whose binary expansion is not rich.
 * https://oeis.org/A348664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A348664 – Numbers whose binary expansion is NOT rich.
   The value is obtained by scanning integers, testing the rich property
   of their binary representation, and memoising the results. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"

/* Return true iff the binary representation of val is NOT rich. */
static bool is_not_rich(seq_t val)
{
    /* Length of the binary representation */
    unsigned k_u = bitlen_seq_t(val);
    if (k_u == 0) return false;           /* val == 0 is not counted */

    size_t k = k_u;

    /* Extract bits from least significant to most significant */
    unsigned char *rev = (unsigned char*)malloc(k);
    seq_t tmp = val;
    for (size_t i = 0; i < k; ++i) {
        rev[i] = (unsigned char)(tmp & (seq_t)1);
        tmp >>= 1;
    }

    /* Reverse to obtain normal left‑to‑right order */
    unsigned char *s = (unsigned char*)malloc(k);
    for (size_t i = 0; i < k; ++i) {
        s[i] = rev[k - 1 - i];
    }
    free(rev);

    /* Build an Eertree to count distinct non‑empty palindromic substrings */
    struct Node {
        int len;          /* length of the palindrome */
        int link;         /* suffix link */
        int nxt[2];       /* transitions on bits 0 and 1 */
    };

    size_t cap = k + 2;
    struct Node *tree = (struct Node*)malloc(cap * sizeof(struct Node));
    size_t sz = 2;            /* two root nodes already in place */

    tree[0].len  = -1;        /* root with imaginary length -1 */
    tree[0].link = 0;
    tree[0].nxt[0] = tree[0].nxt[1] = 0;

    tree[1].len  = 0;         /* empty string root */
    tree[1].link = 0;
    tree[1].nxt[0] = tree[1].nxt[1] = 0;

    int last = 1;             /* longest palindromic suffix */

    for (size_t pos = 0; pos < k; ++pos) {
        int c = s[pos];
        int cur = last;

        /* Find a palindrome that can be extended by s[pos] */
        while (true) {
            int curlen = tree[cur].len;
            if (pos >= (size_t)curlen + 1 &&
                s[pos - 1 - curlen] == c)
                break;
            cur = tree[cur].link;
        }

        /* Create new palindrome if it doesn't exist */
        if (tree[cur].nxt[c] == 0) {
            tree[sz].len = tree[cur].len + 2;

            /* Set suffix link for the new node */
            if (tree[sz].len == 1) {
                tree[sz].link = 1;
            } else {
                int link = tree[cur].link;
                while (true) {
                    int linklen = tree[link].len;
                    if (pos >= (size_t)linklen + 1 &&
                        s[pos - 1 - linklen] == c)
                        break;
                    link = tree[link].link;
                }
                tree[sz].link = tree[link].nxt[c];
            }

            tree[sz].nxt[0] = tree[sz].nxt[1] = 0;
            tree[cur].nxt[c] = (int)sz;
            ++sz;
        }

        last = tree[cur].nxt[c];
    }

    int distinct_non_empty = (int)(sz - 2);   /* exclude the two roots */

    free(s);
    for (size_t i = 0; i < sz; ++i) {
        /* free any allocated substructures if needed (none here) */
    }
    free(tree);

    /* A word is NOT rich iff it has strictly fewer distinct
     * non‑empty palindromic substrings (count) than its length. */
    return distinct_non_empty < (int)k;
}

/* Return the n‑th integer whose binary expansion is not rich. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t tested = 1;       /* next integer to test */

    /* Ensure cache is large enough to hold n elements */
    while (cache_len < (size_t)n) {
        /* Grow the cache array if needed */
        if (cache_cap < cache_len + 16) {
            cache_cap = (cache_len + 16) * 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Test the current integer */
        if (is_not_rich((seq_t)tested)) {
            cache[cache_len++] = (seq_t)tested;
        }
        ++tested;                    /* move to next integer */
    }

    return cache[n - 1];
}
