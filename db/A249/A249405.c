/*
 * A249405  Numbers such that any two adjacent digits sum to a prime and such
 *          that any digit 0,...,9 can be inserted somewhere preserving this
 *          property.
 * https://oeis.org/A249405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 124, u64 n <= 124, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A249405
 *
 * A(n) is the n-th smallest positive integer for which
 *   - every pair of adjacent decimal digits sums to a prime,
 *   - for each digit d ∈ {0,…,9} we can insert d somewhere (prefix if d≠0, otherwise
 *     only suffix or interior) and still have a number satisfying the adjacency rule.
 *
 * The function generates all numbers in ascending order by increasing length and
 * lexicographic order within each length, caching previously computed terms.
 *
 * Complexity:   Roughly O(m · 10^k) where m is the number of admissible numbers
 *                of length k; for n ≤ 10000 this stays well below the time limit.
 *
 * Notes:  The implementation relies only on the arithmetic operators of
 *         seq_t; it works for arbitrary‐precision seq_t classes as well as
 *         the built‑in unsigned integer types.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* adjacency: true if a+b is prime (0…18) */
static bool adj[10][10];

/* Build adjacency matrix once */
static void init_adj(void)
{
    static bool prime[19] = {
        0,0,1,1,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0
    };
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            adj[a][b] = prime[a + b];
}

/* Test whether inserting any digit 0…9 somewhere keeps the adjacency property */
static bool insertion_ok(int len, const uint8_t *dig)
{
    for (int d = 0; d <= 9; ++d) {
        bool ok = false;
        /* prefix (only allowed if d ≠ 0) */
        if (d != 0 && adj[d][dig[0]])
            ok = true;
        /* suffix */
        if (!ok && adj[dig[len - 1]][d])
            ok = true;
        /* interior positions */
        if (!ok) {
            for (int i = 1; i < len; ++i) {
                if (adj[d][dig[i - 1]] && adj[dig[i]][d]) {
                    ok = true;
                    break;
                }
            }
        }
        if (!ok)
            return false;
    }
    return true;
}

/* Cache of generated sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static int next_len_to_gen = 1;   /* next number of digits to generate */

/* Append a value to the cache, growing it if necessary */
static void cache_append(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Depth‑first search to generate all admissible numbers of length 'len' */
static void dfs_gen(int len, int pos, int prev, seq_t val, uint8_t *dig)
{
    if (pos == len) {
        if (insertion_ok(len, dig))
            cache_append(val);
        return;
    }
    for (int nxt = 0; nxt < 10; ++nxt) {
        if (!adj[prev][nxt])
            continue;
        dig[pos] = (uint8_t)nxt;
        dfs_gen(len, pos + 1, nxt, val * (seq_t)10 + (seq_t)nxt, dig);
    }
}

/* Generate all numbers of a given length, adding them to the cache */
static void generate_len(int len)
{
    uint8_t dig[200];   /* enough for all needed lengths */
    if (len == 1) {
        for (int d = 1; d <= 9; ++d) {
            dig[0] = (uint8_t)d;
            if (insertion_ok(1, dig))
                cache_append((seq_t)d);
        }
    } else {
        for (int start = 1; start <= 9; ++start) {
            dig[0] = (uint8_t)start;
            dfs_gen(len, 1, start, (seq_t)start, dig);
        }
    }
}

/* Ensure that the cache contains at least 'n' terms */
static void ensure_cache_size(idx_t n)
{
    if (!adj[0][0])
        init_adj();
    while (cache_len < (size_t)n) {
        generate_len(next_len_to_gen);
        ++next_len_to_gen;
    }
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return cache[n - 1];
}
