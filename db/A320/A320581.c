/*
 * A320581  a(n) is the number of closed factors of length n in the Fibonacci
 *          word.
 * https://oeis.org/A320581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   is_closed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculates the number of closed factors of length n in the Fibonacci word. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_closed.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n == 1) return 2;   /* base case */

    size_t N = (size_t)n;

    /* Build a Fibonacci word long enough to contain all subwords of length N.
     * The condition |F_k| ≥ N + |F_{k-1}| guarantees that the prefix of
     * length |F_k| contains every factor of length N. */
    char *prev = NULL, *curr = NULL;
    size_t prev_len = 0, curr_len = 0;

    prev = (char *)malloc(1);
    prev[0] = '0';
    prev_len = 1;

    curr = (char *)malloc(2);
    curr[0] = '0';
    curr[1] = '1';
    curr_len = 2;

    while (curr_len < N + prev_len) {
        size_t new_len = curr_len + prev_len;
        char *next = (char *)malloc(new_len);
        memcpy(next, curr, curr_len);
        memcpy(next + curr_len, prev, prev_len);

        free(prev);
        prev = curr;
        curr = next;

        prev_len = curr_len;
        curr_len = new_len;
    }

    /* Now 'curr' holds the needed Fibonacci word. */
    char *w = curr;
    size_t total_len = curr_len;

    /* Prefix hashes for O(1) substring comparison. */
    const uint64_t BASE = 1315423911ull;
    uint64_t *pref = (uint64_t *)malloc((total_len + 1) * sizeof(uint64_t));
    pref[0] = 0;
    for (size_t i = 0; i < total_len; ++i)
        pref[i + 1] = pref[i] * BASE + (uint64_t)(w[i] - '0' + 1);

    uint64_t *powb = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    powb[0] = 1;
    for (size_t i = 1; i <= N; ++i)
        powb[i] = powb[i - 1] * BASE;

    /* Open-addressed hash table for distinct substrings. */
    const size_t TABLE_BITS = 17;          /* 131072 slots */
    const size_t TABLE_SIZE = 1u << TABLE_BITS;
    uint64_t *tab_key = (uint64_t *)malloc(TABLE_SIZE * sizeof(uint64_t));
    size_t   *tab_start = (size_t *)malloc(TABLE_SIZE * sizeof(size_t));
    for (size_t i = 0; i < TABLE_SIZE; ++i)
        tab_key[i] = (uint64_t)-1;

    seq_t closed_cnt = 0;
    size_t last_start = total_len - N;
    for (size_t start = 0; start <= last_start; ++start) {
        uint64_t h = pref[start + N] - pref[start] * powb[N];
        size_t idx = h & (TABLE_SIZE - 1);

        while (true) {
            uint64_t key = tab_key[idx];
            if (key == (uint64_t)-1) {
                tab_key[idx] = h;
                tab_start[idx] = start;
                if (is_closed(w + start, N))
                    ++closed_cnt;
                break;
            } else if (key == h) {
                size_t old_start = tab_start[idx];
                /* Confirm equality to avoid hash-collision miscounts. */
                bool eq = true;
                for (size_t t = 0; t < N; ++t) {
                    if (w[start + t] != w[old_start + t]) {
                        eq = false;
                        break;
                    }
                }
                if (eq) break;   /* duplicate, skip counting */
            }
            idx = (idx + 1) & (TABLE_SIZE - 1);
        }
    }

    /* Clean up. */
    free(pref);
    free(powb);
    free(tab_key);
    free(tab_start);
    free(prev);
    free(w);

    return closed_cnt;
}
