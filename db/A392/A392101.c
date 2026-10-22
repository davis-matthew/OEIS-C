/*
 * A392101  Numbers whose square is the concatenation of two or more powers of
 *          2.
 * https://oeis.org/A392101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   double_string, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A392101: numbers whose square is a concatenation of
   two or more powers of 2 (no leading zeros in the powers). */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "../utils/seq_to_str.h"   /* seq_to_str(seq_t v, char *buf, size_t *len) */
#include "../utils/double_string.h"

#define MAX_DIGITS 4096   /* safe upper bound for interim buffers */

/* ---------- Powers of 2 stored as decimal strings ---------- */
static char  **power_strs = NULL;   /* array of strings */
static int     *power_lens = NULL;   /* length of each string */
static size_t   power_count = 0;     /* number used */
static size_t   power_capacity = 0;  /* allocated size */

/* ** Ensure powers[0 .. power_count-1] contain all 2^k with decimal
   length <= minLen ** */
static void extend_powers_up_to_len(int minLen)
{
    if (power_count == 0) {                 /* first allocation */
        power_capacity = 32;
        power_strs = (char **)malloc(power_capacity * sizeof(char *));
        power_lens = (int  *)malloc(power_capacity * sizeof(int));
        power_capacity = 32;
        power_count = 0;
    }

    /* start with 1 if empty */
    if (power_count == 0) {
        power_strs[0] = strdup("1");
        power_lens[0] = 1;
        power_count = 1;
    }

    /* generate the next powers by doubling until we reach minLen */
    while (power_lens[power_count - 1] < minLen) {
        /* double the last power string */
        int last_len = power_lens[power_count - 1];
        char *next_str;
        int  next_len;
        double_string(power_strs[power_count - 1], last_len,
                      &next_str, &next_len);

        /* grow arrays if needed */
        if (power_count >= power_capacity) {
            size_t new_cap = power_capacity * 2;
            char **tmp_str = (char **)realloc(power_strs,
                                              new_cap * sizeof(char *));
            int  *tmp_len = (int  *)realloc(power_lens,
                                              new_cap * sizeof(int));
            power_strs = tmp_str;
            power_lens = tmp_len;
            power_capacity = new_cap;
        }

        power_strs[power_count] = next_str;
        power_lens[power_count] = next_len;
        power_count++;
    }
}

/* ---------- Sequence terms cached ---------- */
static seq_t *term_arr    = NULL;   /* array of terms */
static size_t term_count   = 0;      /* number of terms computed */
static size_t term_capacity = 0;    /* allocated size */

static idx_t next_candidate = 1;     /* next integer to test */

/* ** Ensure we have at least 'need' terms computed ** */
static void ensure_terms(idx_t need)
{
    if (term_capacity == 0) {
        term_capacity = 64;
        term_arr = (seq_t *)malloc(term_capacity * sizeof(seq_t));
        term_count = 0;
    }
    if (need <= term_count) return;

    /* buffer for square as decimal string */
    char buf[MAX_DIGITS];
    size_t blen;

    while (term_count < need) {
        /* square of candidate */
        seq_t m_seq = (seq_t)next_candidate;
        seq_t sq = m_seq * m_seq;           /* works for builtin and class */

        /* get decimal representation */
        seq_to_str(sq, buf, &blen);
        if (blen > MAX_DIGITS) blen = MAX_DIGITS;   /* safety limit */

        /* ensure we have powers up to this length */
        extend_powers_up_to_len((int)blen);

        /* DP: parts[i] = minimal number of power‑of‑2 pieces for suffix
           starting at position i.  INF means impossible. */
        int INF = INT_MAX / 2;
        int *parts = (int *)malloc((blen + 1) * sizeof(int));
        for (size_t i = 0; i <= blen; ++i) parts[i] = INF;
        parts[blen] = 0;

        for (int i = (int)blen - 1; i >= 0; --i) {
            for (size_t pi = 0; pi < power_count; ++pi) {
                int p_len = power_lens[pi];
                if (p_len > blen - i) continue;
                if (memcmp(buf + i, power_strs[pi], p_len) == 0) {
                    int candidate = parts[i + p_len] + 1;
                    if (candidate < parts[i]) parts[i] = candidate;
                }
            }
        }

        /* accept if at least two pieces and a valid decomposition exists */
        bool ok = (parts[0] != INF && parts[0] >= 2);
        free(parts);

        if (ok) {
            /* store term */
            if (term_count >= term_capacity) {
                size_t new_cap = term_capacity * 2;
                seq_t *tmp = (seq_t *)realloc(term_arr,
                                              new_cap * sizeof(seq_t));
                term_arr = tmp;
                term_capacity = new_cap;
            }
            term_arr[term_count++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_terms(n);
    return term_arr[n - 1];
}
