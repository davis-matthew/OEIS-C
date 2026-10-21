/*
 * A255724  List of numbers such that no number, nor its reverse, is in the
 *          concatenation of all previous terms.
 * https://oeis.org/A255724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A255724 via incremental concatenation and substring search */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "../utils/seq_from_str.h"

static bool initialized = false;
static seq_t *seq_arr = NULL;
static size_t seq_cap = 0;
static idx_t seq_len = 0;
static uint64_t candidate = 1;

static char *cat_buf = NULL;
static size_t cat_len = 0;
static size_t cat_cap = 0;

static bool contains(const char *h, size_t hl, const char *p, size_t pl)
{
    if (pl == 0) return true;
    if (pl > hl) return false;
    for (size_t i = 0; i + pl <= hl; ++i) {
        if (h[i] == p[0] && memcmp(h + i, p, pl) == 0)
            return true;
    }
    return false;
}

static void grow_seq(void)
{
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void grow_cat(size_t need)
{
    size_t new_cap = cat_cap ? cat_cap * 2 : 256;
    while (new_cap < need) new_cap *= 2;
    cat_buf = (char *)realloc(cat_buf, new_cap);
    cat_cap = new_cap;
}

static void ensure_computed(idx_t n)
{
    if (!initialized) initialized = true;
    while (seq_len < n) {
        char buf[64];
        snprintf(buf, sizeof(buf), "%llu", (unsigned long long)candidate);
        size_t len = strlen(buf);

        char rev[64];
        for (size_t i = 0; i < len; ++i) rev[i] = buf[len - 1 - i];
        rev[len] = '\0';

        if (!contains(cat_buf, cat_len, buf, len) &&
            !contains(cat_buf, cat_len, rev, len)) {

            if (seq_len >= seq_cap) grow_seq();
            seq_arr[seq_len] = seq_from_str(buf);
            ++seq_len;

            size_t need = cat_len + len;
            if (need > cat_cap) grow_cat(need);
            memcpy(cat_buf + cat_len, buf, len);
            cat_len = need;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */
    ensure_computed(n);
    return seq_arr[n - 1];
}
