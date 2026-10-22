/*
 * A348302  Lexicographically earliest sequence of distinct terms such that
 *          the sum of two neighboring digits is not a palindrome.
 * https://oeis.org/A348302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = lexicographically earliest distinct integer that can be
 *            appended so that the sum of every pair of adjacent digits in the
 *            concatenated sequence is not a palindrome. (Zero digits are
 *            forbidden.)
 * Range:     u8 n <= 47, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   allowed_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/allowed_sum.h"

/* Static storage for the generated sequence and meta‑information. */
static seq_t *seq_arr  = NULL;   /* array of terms */
static size_t seq_len  = 0;      /* number of terms stored */
static size_t seq_cap  = 0;      /* capacity of seq_arr */
static seq_t  last_digit = 0;    /* last digit of the last term (valid if seq_len>0) */

/* Ensure that the sequence contains at least 'need' terms. */
static void ensure_len(idx_t need)
{
    if (seq_len >= need)
        return;

    if (seq_cap == 0) {
        seq_cap = 16;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    while (seq_len < need) {
        seq_t cand = 1;              /* start search from the smallest integer */
        while (true) {
            /* skip already used terms */
            bool used = false;
            for (size_t i = 0; i < seq_len; ++i) {
                if (seq_arr[i] == cand) {
                    used = true;
                    break;
                }
            }
            if (used) {
                cand++;
                continue;
            }

            if (cand == (seq_t)0) {  /* zero is forbidden */
                cand++;
                continue;
            }

            /* Extract digits of cand in MSB → LSB order,
               and check for forbidden zero digits. */
            seq_t t = cand;
            int digits[128];
            int dlen = 0;
            bool bad = false;

            while (t != (seq_t)0) {
                int d = (int)(t % (seq_t)10);
                if (d == 0) {
                    bad = true;
                    break;
                }
                digits[dlen++] = d;
                t /= (seq_t)10;
            }
            if (bad) {
                cand++;
                continue;
            }
            if (dlen == 0) { /* cand == 0, already handled but keep */
                cand++;
                continue;
            }

            /* Reverse to obtain MSB first order */
            for (int i = 0; i < dlen / 2; ++i) {
                int tmp = digits[i];
                digits[i] = digits[dlen - 1 - i];
                digits[dlen - 1 - i] = tmp;
            }

            int first_digit  = digits[0];
            int cur_last     = digits[dlen - 1];

            /* Cross‑term sum with previous term's last digit, if any */
            if (seq_len > 0) {
                if (!allowed_sum((int)last_digit, first_digit)) {
                    cand++;
                    continue;
                }
            }

            /* Internal sums within the candidate number */
            for (int i = 0; i < dlen - 1; ++i) {
                if (!allowed_sum(digits[i], digits[i + 1])) {
                    bad = true;
                    break;
                }
            }
            if (bad) {
                cand++;
                continue;
            }

            /* Accept the candidate */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
            }
            seq_arr[seq_len] = cand;
            seq_len++;
            last_digit = cur_last;
            break; /* move to next required term */
        }
    }
}

seq_t a(idx_t n)
{
    /* The sequence is 1‑based. */
    ensure_len(n);
    return seq_arr[n - 1];
}
