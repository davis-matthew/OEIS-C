/*
 * A286750  Positions of 0 in A286749; complement of A286751.
 * https://oeis.org/A286750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + Σ_{k=1}^{n-1} d_k where d_k is the k‑th digit of the
 *            word obtained by decorating the Fibonacci word with a->14,
 *            b->13. For each pair, the first digit is always 1, the second
 *            digit is 4 if the letter is a and 3 if the letter is b
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* static state */
    static seq_t *arr = NULL;        /* array of computed a-values */
    static size_t arr_len  = 0;       /* number of stored values */
    static size_t arr_cap  = 0;       /* capacity of the array */

    static char *w_prev = NULL;      /* previous Fibonacci word */
    static size_t len_prev = 0;
    static char *w_cur  = NULL;      /* current (longer) Fibonacci word */
    static size_t len_cur  = 0;
    static seq_t last_val = 0;       /* a(arr_len) */

    /* initialisation on first call */
    if (arr_len == 0) {
        /* store the first term a(1)=3 */
        arr_cap = 16;
        arr = (seq_t*)malloc(arr_cap * sizeof(seq_t));
        arr[0] = (seq_t)3;
        arr_len = 1;
        last_val = (seq_t)3;

        /* initial Fibonacci words */
        len_prev = 1;
        w_prev = (char*)malloc(len_prev);
        w_prev[0] = 'b';

        len_cur = 1;
        w_cur = (char*)malloc(len_cur);
        w_cur[0] = 'a';
    }

    /* return if already known */
    if (n <= arr_len)
        return arr[n-1];

    /* grow the array if necessary */
    while (arr_len < n) {
        idx_t diff_index = arr_len;          /* paper = a(arr_len+1) - a(arr_len) */
        size_t letter_idx = (diff_index-1)/2;
        int offset = (diff_index-1)%2;

        /* ensure we have the required letter in w_cur */
        size_t needed_letters = letter_idx + 1;
        while (len_cur < needed_letters) {
            size_t new_len = len_cur + len_prev;
            char *new_word = (char*)malloc(new_len);
            memcpy(new_word, w_cur, len_cur);
            memcpy(new_word + len_cur, w_prev, len_prev);
            free(w_prev);
            w_prev = w_cur;
            w_cur  = new_word;
            len_prev = len_cur;
            len_cur  = new_len;
        }

        /* get the letter and compute the difference value */
        char letter = w_cur[letter_idx];
        seq_t diff_val = (offset==0) ? (seq_t)1 :
                         (letter=='a' ? (seq_t)4 : (seq_t)3);

        /* append new a-value */
        seq_t new_val = last_val + diff_val;
        if (arr_len >= arr_cap) {
            size_t new_cap = arr_cap * 2;
            seq_t *tmp = (seq_t*)realloc(arr, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed – abort */
                exit(1);
            }
            arr = tmp;
            arr_cap = new_cap;
        }
        arr[arr_len++] = new_val;
        last_val = new_val;
    }

    return arr[n-1];
}
