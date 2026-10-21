/*
 * A257234  Levels of Narayana tree written as rows of an irregular triangle.
 * https://oeis.org/A257234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   not stated
 * Range:     i8 n <= 125, i16 n <= 125, i32 n <= 125, i64 n <= 125,
 *            i128 n <= 125, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257234 – Levels of Narayana tree written as rows of an irregular triangle
   Generation:
   NW(0) = "1"          (length 1)
   NW(1) = "10"         (length 2)
   NW(2) = "100"        (length 3)
   NW(3) = "100-1"      (length 4)
   For n ≥ 4: NW(n) = NW(n‑1) concatenated with NW(n‑3).
   The infinite sequence a(n) is the n‑th symbol (1, 0 or –1) of the
   concatenation of all rows NW(0), NW(1), …
   The row lengths L(n) satisfy L(n) = L(n‑1) + L(n‑3) with
   L(0)=1, L(1)=2, L(2)=3.  */

#include <stdlib.h>   /* malloc, realloc */

static seq_t value_at(idx_t r, idx_t p, const idx_t *len) {
    /* Return the p‑th symbol of row r (0‑based). */
    if (r == 0) {                    /* [1] */
        return (seq_t)1;
    } else if (r == 1) {             /* [1,0] */
        return (seq_t)(p == 0 ? 1 : 0);
    } else if (r == 2) {             /* [1,0,0] */
        return (seq_t)(p == 0 ? 1 : 0);
    } else if (r == 3) {             /* [1,0,0,-1] */
        switch (p) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)0;
        case 2: return (seq_t)0;
        case 3: return (seq_t)-1;
        default: return (seq_t)0;    /* unreachable */
        }
    } else {                         /* r ≥ 4: NW(r) = NW(r‑1) || NW(r‑3) */
        if (p < len[r-1]) {
            return value_at(r-1, p, len);
        } else {
            return value_at(r-3, p - len[r-1], len);
        }
    }
}

seq_t a(idx_t n) {
    /* Store row lengths once.  The array is enlarged as needed. */
    static idx_t *len = NULL;
    static size_t len_size = 0; /* number of lengths already stored */
    static size_t len_cap  = 0; /* capacity of the array */

    idx_t r = 0;      /* current row index */
    idx_t cum = 0;    /* total length of all rows < r */

    /* Find the row r that contains the global index n. */
    while (1) {
        /* Ensure len[r] is known. */
        while (len_size <= r) {
            idx_t new_len;
            if (r == 0) {
                new_len = 1;
            } else if (r == 1) {
                new_len = 2;
            } else if (r == 2) {
                new_len = 3;
            } else if (r == 3) {
                new_len = 4;
            } else {
                new_len = len[r-1] + len[r-3];
            }

            if (len_size == len_cap) {
                size_t new_cap = len_cap ? len_cap * 2 : 8;
                idx_t *tmp = (idx_t *)realloc(len, new_cap * sizeof(idx_t));
                if (!tmp) {
                    free(len);
                    len = NULL;
                    len_size = len_cap = 0;
                    return (seq_t)0;   /* allocation failure – return 0 */
                }
                len = tmp;
                len_cap = new_cap;
            }
            len[len_size++] = new_len;
        }

        idx_t l = len[r];
        if (cum + l > n) {
            break;  /* n lies in this row */
        }
        cum += l;
        r++;
    }

    /* Local position inside row r. */
    idx_t pos = n - cum;
    return value_at(r, pos, len);
}
