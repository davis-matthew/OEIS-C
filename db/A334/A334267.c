/*
 * A334267  Numbers k such that the k-th composition in standard order is both
 *          a Lyndon word and a reversed co-Lyndon word.
 * https://oeis.org/A334267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute-force search: a(n) is the n-th integer k whose binary
   representation is a reversed Lyndon word and a co‑Lyndon word. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Internal helper: check whether a binary string of length len is a Lyndon word. */
static bool is_lyndon(const char *s, int len)
{
    if (len <= 1) return true;

    /* KMP prefix function to test primitiveness */
    int pi[128];
    pi[0] = 0;
    for (int i = 1; i < len; ++i) {
        int j = pi[i-1];
        while (j > 0 && s[i] != s[j]) j = pi[j-1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }
    int period = len - pi[len-1];
    if (period < len && len % period == 0) return false; /* not primitive */

    /* Duval algorithm to find the lexicographically least rotation */
    int i = 0, j = 1, k = 0;
    while (i < len && j < len && k < len) {
        char a = s[(i + k) % len];
        char b = s[(j + k) % len];
        if (a == b) { ++k; continue; }
        if (a > b) { i = i + k + 1; if (i <= j) i = j + 1; }
        else { j = j + k + 1; if (j <= i) j = i + 1; }
        k = 0;
    }
    int rot = (i < j) ? i : j;
    return rot == 0;
}

/* Internal helper: check whether a binary string of length len is a co‑Lyndon word. */
static bool is_co_lyndon(const char *s, int len)
{
    if (len <= 1) return true;
    for (int shift = 1; shift < len; ++shift) {
        int cmp = 0;
        for (int i = 0; i < len; ++i) {
            char a = s[i];
            char b = s[(shift + i) % len];
            if (a < b) { cmp = -1; break; }
            if (a > b) { cmp = 1; break; }
        }
        if (cmp <= 0) return false; /* s <= some rotation */
    }
    return true;
}

/* Test whether k satisfies the definition. */
static bool good_k(idx_t k)
{
    if (k == 0) return true; /* 0 is the first element of the sequence */

    int len = 64 - __builtin_clzll(k);   /* length of binary representation */
    char bits[64];
    for (int i = 0; i < len; ++i)
        bits[i] = ((k >> (len - 1 - i)) & 1ULL) ? '1' : '0';

    if (!is_co_lyndon(bits, len)) return false;

    char rev[64];
    for (int i = 0; i < len; ++i) rev[i] = bits[len - 1 - i];
    return is_lyndon(rev, len);
}

/* Static cache for previously computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t next_k = 0;   /* next integer to test */
static idx_t found = 0;     /* number of terms already found */

/* Ensure the cache can hold at least 'need' terms. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!ptr) exit(EXIT_FAILURE);
    cache = ptr;
    cache_cap = new_cap;
}

/* External function required by the build. */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety guard */

    while (found < n) {
        if (good_k(next_k)) {
            ensure_cache(found + 1);
            cache[found] = (seq_t)next_k;
            ++found;
        }
        ++next_k;
    }
    return cache[n-1];
}
