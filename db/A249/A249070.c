/*
 * A249070  a(n+1) gives the number of occurrences of the maximum digit of
 *          a(n) in factorial base (i.e., A246359(a(n))) so far amongst the
 *          factorial base representations of all the terms up to and
 *          including a(n), with a(0)=0.
 * https://oeis.org/A249070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 177, u16 n <= 8846, u32 n <= 8846, u64 n <= 8846,
 *            u128 n <= 8846, big any n
 * Helpers:   max_digit_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0.
   For n ≥ 0, let d be the maximum digit in the factorial‑base representation of a(n);
   then a(n+1) is the total number of occurrences of d in all factorial‑base representations
   of a(0), a(1), …, a(n). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/max_digit_factorial.h"

seq_t a(idx_t n)
{
    /* Static state for memoisation ------------------------------------- */
    static seq_t *seq   = NULL;      /* array of computed terms      */
    static size_t seq_cap = 0;       /* capacity of seq array        */
    static size_t len     = 0;       /* number of terms stored       */

    static unsigned long long *digitcnt = NULL; /* counts for digits 0.. */
    static size_t cnt_cap = 0;                  /* capacity of digitcnt   */

    /* One‑time initialisation ------------------------------------------ */
    if (seq == NULL) {
        seq_cap = 1;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = 0;
        len = 1;

        cnt_cap = 16;
        digitcnt = (unsigned long long *)malloc(cnt_cap * sizeof(unsigned long long));
        memset(digitcnt, 0, cnt_cap * sizeof(unsigned long long));
        digitcnt[0] = 1;          /* digit 0 occurs once in number 0 */
    }

    /* Compute terms up to and including n ------------------------------ */
    while (len <= n) {
        seq_t curr = seq[len - 1];

        /* 1. Find maximum digit of current term */
        unsigned int maxd = max_digit_factorial(curr);

        /* Ensure digitcnt array can hold index maxd */
        if ((size_t)maxd >= cnt_cap) {
            size_t new_cap = cnt_cap ? cnt_cap : 1;
            while ((size_t)maxd >= new_cap) new_cap <<= 1;
            unsigned long long *tmp = (unsigned long long *)realloc(digitcnt, new_cap * sizeof(unsigned long long));
            memset(tmp + cnt_cap, 0, (new_cap - cnt_cap) * sizeof(unsigned long long));
            digitcnt = tmp;
            cnt_cap = new_cap;
        }

        /* 2. Determine next term: current count of digit maxd */
        seq_t next = (seq_t)digitcnt[maxd];

        /* Ensure seq array has space for next term */
        if (len >= seq_cap) {
            size_t new_cap = seq_cap ? seq_cap << 1 : 1;
            while (new_cap <= len) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            seq = tmp;
            seq_cap = new_cap;
        }
        seq[len++] = next;

        /* 3. Update digit counts with representation of new term */
        seq_t x = next;
        if (x == 0) {
            /* representation of zero is a single digit 0 */
            ++digitcnt[0];
        } else {
            seq_t tmp_val = x;
            unsigned int k = 1;
            while (tmp_val > 0) {
                seq_t divisor = (seq_t)(k + 1);
                unsigned int d = (unsigned int)(tmp_val % divisor);

                if ((size_t)d >= cnt_cap) {
                    size_t new_cap = cnt_cap ? cnt_cap : 1;
                    while ((size_t)d >= new_cap) new_cap <<= 1;
                    unsigned long long *tmp = (unsigned long long *)realloc(digitcnt, new_cap * sizeof(unsigned long long));
                    memset(tmp + cnt_cap, 0, (new_cap - cnt_cap) * sizeof(unsigned long long));
                    digitcnt = tmp;
                    cnt_cap = new_cap;
                }
                ++digitcnt[d];

                tmp_val = tmp_val / divisor;
                ++k;
            }
        }
    }

    return seq[n];
}
