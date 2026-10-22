/*
 * A355791  Numbers that can be written as the product of two divisors greater
 *          than 1 such that the number in binary is contained in the string
 *          concatenation of the divisors in binary.
 * https://oeis.org/A355791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   bitlen_seq_t, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A355791: the n‑th integer m that can be written as
 * m = d1 * d2  (d1,d2 > 1) and the binary representation of m is a
 * substring of the concatenation of the binary representations of
 * d1 and d2 (in either order).  a(1) = 6, a(2) = 10, a(3) = 12, ...  */

/*  The code determines the sequence by generating candidates in
 *  increasing order and keeping a memoised table of the first n values.
 *  It works for all supported integer types – built‑in unsigned
 *  integers, as well as the arbitrary‑precision seq_t class – by
 *  using only the operations allowed on seq_t and by casting the results
 *  of malloc/realloc explicitly to seq_t*. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/floor_sqrt.h"

static inline void bin_str(seq_t x, char *out, size_t *len)
{
    size_t l = bitlen_seq_t(x);
    *len = l;
    for (size_t i = 0; i < l; ++i) {
        unsigned long long idx = i;
        out[l - 1 - i] = ((x >> idx) & 1ULL) ? '1' : '0';
    }
}

static inline bool contains_substring(const char *txt, size_t txt_len,
                                      const char *pat, size_t pat_len)
{
    if (pat_len == 0) return true;
    if (txt_len < pat_len) return false;
    for (size_t i = 0; i + pat_len <= txt_len; ++i) {
        bool ok = true;
        for (size_t j = 0; j < pat_len; ++j) {
            if (txt[i + j] != pat[j]) { ok = false; break; }
        }
        if (ok) return true;
    }
    return false;
}

static bool check_m(seq_t m)
{
    char m_str[257];
    size_t len_m;
    bin_str(m, m_str, &len_m);

    seq_t limit = floor_sqrt(m);
    for (seq_t d1 = 2; d1 <= limit; ++d1) {
        if (m % d1 != 0) continue;
        seq_t d2 = m / d1;
        if (d2 < 2) continue;

        char s1[257], s2[257];
        size_t len1, len2;
        bin_str(d1, s1, &len1);
        bin_str(d2, s2, &len2);

        /* d1 || d2 */
        char concat12[514];
        memcpy(concat12, s1, len1);
        memcpy(concat12 + len1, s2, len2);
        if (contains_substring(concat12, len1 + len2, m_str, len_m))
            return true;

        /* d2 || d1 */
        char concat21[514];
        memcpy(concat21, s2, len2);
        memcpy(concat21 + len2, s1, len1);
        if (contains_substring(concat21, len1 + len2, m_str, len_m))
            return true;
    }
    return false;
}

static seq_t *seq_arr = NULL;
static size_t seq_size = 0;
static size_t seq_cap = 0;
static seq_t next_candidate = 6;

static void ensure_terms(idx_t n)
{
    if (seq_size >= n) return;

    if (seq_cap == 0) {
        seq_cap = 64;
        seq_arr = (seq_t*)malloc(sizeof(seq_t) * seq_cap);
    } else {
        while (seq_cap < n) {
            seq_cap <<= 1;
            seq_arr = (seq_t*)realloc(seq_arr, sizeof(seq_t) * seq_cap);
        }
    }

    while (seq_size < n) {
        if (check_m(next_candidate))
            seq_arr[seq_size++] = next_candidate;
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but avoid UB */
    ensure_terms(n);
    return seq_arr[n - 1];
}
