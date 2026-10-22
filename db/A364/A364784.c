/*
 * A364784  a(n) = n for n <= 2. Thereafter if a(n-1) is a novel term, a(n) =
 *          a(a(k)) where k is the greatest prior term < a(n-1); otherwise,
 *          a(n) = number of times a(n-1) has been repeated.
 * https://oeis.org/A364784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 4375, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1, a(2) = 2.
   For n > 2:
     Let p = a(n-1).
     If p has appeared exactly once before (i.e., it is novel), let k be the
     greatest earlier term that is strictly less than p.  Then
          a(n) = a(a(k)).
     Otherwise a(n) = the number of times p has been repeated,
     i.e. (count of p up to n-1) – 1.
   Offset: 1.                                                          */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for the sequence */
static seq_t *seq_buf = NULL;   /* 1‑based index */
static idx_t  seq_cap  = 0;     /* allocated size of seq_buf   */
static idx_t  computed_max = 0; /* largest index that has been computed */

/* enlarge the sequence buffer if needed */
static void ensure_seq_capacity(idx_t need)
{
    if (need <= seq_cap) return;
    idx_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_buf = tmp;
    seq_cap = new_cap;
}

/* main OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined below offset */

    /* one‑time initialization */
    if (!seq_buf) {
        ensure_seq_capacity(3);           /* at least indices 0,1,2 */
        seq_buf[1] = (seq_t)1;
        seq_buf[2] = (seq_t)2;
        computed_max = 2;
    }

    /* extend cache if required */
    if (n > computed_max) {
        ensure_seq_capacity(n + 1);                     /* indices up to n */

        for (idx_t i = computed_max + 1; i <= n; ++i) {
            if (i == 1) { seq_buf[i] = (seq_t)1; continue; }
            if (i == 2) { seq_buf[i] = (seq_t)2; continue; }

            seq_t prev = seq_buf[i - 1];

            idx_t   count_prev = 0;          /* number of occurrences of prev */
            seq_t   greatest_less = 0;
            bool    found_less   = false;

            /* scan previous terms once */
            for (idx_t j = i - 1; j; --j) { /* j>0 guard for unsigned */
                seq_t val_j = seq_buf[j];
                if (val_j == prev) {
                    ++count_prev;
                }
                if (val_j < prev) {
                    if (!found_less || val_j > greatest_less) {
                        greatest_less = val_j;
                        found_less = true;
                    }
                }
            }

            seq_t out;
            if (count_prev == 1) { /* novel */
                /* k is the greatest prior term less than prev */
                idx_t k = (idx_t)greatest_less;
                out = seq_buf[k];
            } else {                 /* repeated */
                out = (seq_t)(count_prev - 1);  /* number of repeats */
            }

            seq_buf[i] = out;
        }

        computed_max = n;
    }

    return seq_buf[n];
}
