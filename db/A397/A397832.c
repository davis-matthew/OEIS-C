/*
 * A397832  Number of distinct period sets of the length-n blocks appearing in
 *          the infinite Fibonacci word A003849.
 * https://oeis.org/A397832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct period sets of the length‑n blocks
 *            appearing in the infinite Fibonacci word A003849. The algorithm
 *            enumerates all n‑length factors of a sufficiently long prefix of
 *            the Fibonacci word, computes all periods of each factor via the
 *            KMP prefix function, and records each uniqu...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   next_fib_greater
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/next_fib_greater.h"

/* Global Fibonacci word buffer and its length. */
static char *fib_word = NULL;
static size_t fib_len = 0;

/* Extend fib_word until it has length at least min_len. */
static void ensure_fib_word_size(size_t min_len)
{
    if (fib_len >= min_len)
        return;

    if (fib_len == 0) {
        fib_word = (char *)malloc(2);
        fib_word[0] = '0';
        fib_word[1] = '1';
        fib_len = 2;
        return;
    }

    while (fib_len < min_len) {
        /* Count zeros to determine next length. */
        size_t zeros = 0;
        for (size_t i = 0; i < fib_len; ++i)
            if (fib_word[i] == '0')
                ++zeros;

        size_t new_len = fib_len + zeros;
        char *new_word = (char *)malloc(new_len);
        size_t pos = 0;
        for (size_t i = 0; i < fib_len; ++i) {
            if (fib_word[i] == '0') {
                new_word[pos++] = '0';
                new_word[pos++] = '1';
            } else {
                new_word[pos++] = '0';
            }
        }

        free(fib_word);
        fib_word = new_word;
        fib_len  = new_len;
    }
}

/* Count the number of distinct period sets among all length‑n windows
   of the current Fibonacci word prefix. */
static seq_t count_distinct_sets(idx_t n)
{
    size_t ns = (size_t)n;               /* local copy as size_t */
    if (fib_len < ns)
        return 0;                        /* safety check */

    /* Prepare data structures. */
    int *pi = (int *)malloc(ns * sizeof(int));
    if (!pi)
        return 0;
    int *periods = (int *)malloc((ns > 0 ? ns - 1 : 0) * sizeof(int));
    if (!periods) {
        free(pi);
        return 0;
    }

    /* Hash table for unique period sets. */
    /* Use power of two size for simple modulo. */
    enum { HASH_SIZE = 32768 };
    struct Node {
        size_t len;
        uint64_t hash;
        int *periods;
        struct Node *next;
    };
    struct Node *table[HASH_SIZE] = {0};

    size_t unique_sets = 0;

    size_t start_limit = fib_len - ns;   /* last starting index */

    for (size_t start = 0; start <= start_limit; ++start) {
        /* Build prefix function for this window. */
        int j = 0;
        pi[0] = 0;
        for (size_t k = 1; k < ns; ++k) {
            while (j > 0 && fib_word[start + k] != fib_word[start + j])
                j = pi[j - 1];
            if (fib_word[start + k] == fib_word[start + j])
                ++j;
            pi[k] = j;
        }

        /* Derive all periods from the border chain. */
        size_t periods_cnt = 0;
        int l = pi[ns - 1];
        while (l > 0) {
            periods[periods_cnt++] = (int)(ns - l);
            l = pi[l - 1];
        }

        /* Compute hash of the period set. */
        uint64_t h = 14695981039346656037ULL; /* FNV offset basis */
        for (size_t i = 0; i < periods_cnt; ++i) {
            h ^= (uint64_t)periods[i];
            h *= 1099511628211ULL;          /* FNV prime */
        }
        size_t idx = (size_t)h & (HASH_SIZE - 1);

        /* Search for duplicate set. */
        struct Node *node = table[idx];
        int found = 0;
        while (node) {
            if (node->hash == h && node->len == periods_cnt) {
                int same = 1;
                for (size_t i = 0; i < periods_cnt; ++i) {
                    if (node->periods[i] != periods[i]) {
                        same = 0;
                        break;
                    }
                }
                if (same) {
                    found = 1;
                    break;
                }
            }
            node = node->next;
        }

        /* Insert new unique set. */
        if (!found) {
            struct Node *newnode = (struct Node *)malloc(sizeof(struct Node) + periods_cnt * sizeof(int));
            newnode->len = periods_cnt;
            newnode->hash = h;
            newnode->periods = (int *)(newnode + 1);
            memcpy(newnode->periods, periods, periods_cnt * sizeof(int));
            newnode->next = table[idx];
            table[idx] = newnode;
            ++unique_sets;
        }
    }

    free(pi);
    free(periods);
    return (seq_t)unique_sets;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Ensure the Fibonacci word is long enough. */
    size_t target_len = (size_t)n + 4ULL * (size_t)next_fib_greater(n);
    ensure_fib_word_size(target_len);

    return count_distinct_sets(n);
}
