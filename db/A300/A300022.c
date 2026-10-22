/*
 * A300022  For any k, the cumulative sum a(1) + a(2) + a(3) + ... + a(k)
 *          shows at least a digit 8.
 * https://oeis.org/A300022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 2208, u32 n <= 8828, u64 n <= 8828,
 *            u128 n <= 8828, big any n
 * Helpers:   has_digit_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A300022: a(1) = 8; for n≥2, a(n) is the smallest unused integer such that
   the cumulative sum a(1)+…+a(n) contains the digit 8 in its decimal
   representation.  The sequence is a permutation of the natural numbers. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/has_digit_8.h"

static size_t seq_len = 0;      /* number of terms currently stored    */
static size_t seq_cap = 0;      /* capacity of seq array                */
static seq_t *seq = NULL;      /* array holding the sequence           */

static size_t used_cap = 0;     /* capacity of used array               */
static bool *used = NULL;      /* marks whether a number has been used */

static seq_t seq_sum = 0;       /* current cumulative sum              */

/* Ensure that 'seq' has capacity for at least 'need' elements. */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure that 'used' has capacity for index 'need-1' (i.e. at least 'need' entries). */
static void ensure_used_capacity(size_t need)
{
    if (need <= used_cap) return;
    size_t old_cap = used_cap;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) abort();
    /* initialise newly allocated part to false */
    memset(tmp + old_cap, 0, (new_cap - old_cap) * sizeof(bool));
    used = tmp;
    used_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 */

    /* Initialise with the first term on first call. */
    if (seq_len == 0) {
        ensure_seq_capacity(16);
        ensure_used_capacity(16);
        seq[0] = (seq_t)8;
        seq_len = 1;
        seq_sum = (seq_t)8;
        ensure_used_capacity(9);
        memset(used, 0, used_cap * sizeof(bool));
        used[8] = true;
    }

    /* Generate terms up to the requested index. */
    while (seq_len < (size_t)n) {
        idx_t cand = 1;
        for (;;) {
            /* Make sure the candidates are represented in 'used'. */
            ensure_used_capacity((size_t)cand + 1);
            if (!used[(size_t)cand]) {
                seq_t new_sum = seq_sum + (seq_t)cand;
                if (has_digit_8(new_sum)) {
                    /* Accept this candidate. */
                    seq_sum = new_sum;
                    ensure_seq_capacity(seq_len + 1);
                    seq[seq_len] = (seq_t)cand;
                    seq_len++;
                    used[(size_t)cand] = true;
                    break;
                }
            }
            ++cand;
        }
    }

    return seq[n - 1];
}
