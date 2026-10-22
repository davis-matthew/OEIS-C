/*
 * A394514  a(1) = 1, a(2) = 2; for n > 2, a(n) is the greatest unused number
 *          less than a(n-2) that shares a factor with a(n-1), else, if no
 *          such number exists, a(n) is the smallest unused number greater
 *          than a(n-2) that shares a factor with a(n-1).
 * https://oeis.org/A394514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; for n>2, a(n) is the greatest unused number <
 *            a(n-2) that shares a factor with a(n-1); if no such number
 *            exists, a(n) is the smallest unused number > a(n-2) that shares
 *            a factor with a(n-1)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"

static seq_t *seq = NULL;          /* stored terms */
static size_t seq_cap = 0;         /* capacity of seq array   */
static size_t seq_len = 0;         /* number of terms stored  */

static uint8_t *used = NULL;       /* used[i] == 1 if i already used   */
static size_t used_cap = 0;        /* capacity of used array           */

static void ensure_seq_cap(size_t n) {
    if ((size_t)n > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        while (new_cap < n) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq) abort();
        seq_cap = new_cap;
    }
}

static void ensure_used_cap(idx_t val) {
    size_t v = (size_t)val;
    if (v >= used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 4;
        while (new_cap <= v) new_cap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
        if (!tmp) abort();
        memset(tmp + used_cap, 0, new_cap - used_cap);
        used = tmp;
        used_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) abort();               /* offset starts at 1                */

    ensure_seq_cap((size_t)n);

    for (size_t i = seq_len; i < (size_t)n; ++i) {
        seq_t val = 0;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)2;
        } else {
            seq_t prev1 = seq[i - 1];
            seq_t prev2 = seq[i - 2];

            seq_t candidate = 0;
            bool found = false;

            /* find greatest unused < prev2 sharing a factor with prev1 */
            if (prev2 > (seq_t)1) {
                for (idx_t j = (idx_t)prev2; j-- > 1; ) {
                    seq_t cand = (seq_t)j;
                    ensure_used_cap(j);
                    if (!used[(size_t)j] && gcd(prev1, cand) != (seq_t)1) {
                        candidate = cand;
                        found = true;
                        break;
                    }
                }
            }

            /* if not found, find smallest unused > prev2 sharing a factor */
            if (!found) {
                for (idx_t j = (idx_t)prev2 + 1; ; ++j) {
                    seq_t cand = (seq_t)j;
                    ensure_used_cap(j);
                    if (!used[(size_t)j] && gcd(prev1, cand) != (seq_t)1) {
                        candidate = cand;
                        break;
                    }
                }
            }

            val = candidate;
        }

        /* store the new term and mark it used */
        seq[i] = val;
        ensure_used_cap((idx_t)val);
        used[(size_t)val] = 1;
        seq_len = i + 1;
    }

    return seq[n - 1];
}
