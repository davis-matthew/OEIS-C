/*
 * A260099  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 00000101 or 00010101.
 * https://oeis.org/A260099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            2*a(n‑2)+2*a(n‑3)+11*a(n‑4)+6*a(n‑5)+2*a(n‑6)+6*a(n‑7)-8*a(n‑8)+2*a(n‑9)
 *            a(1)=48, a(2)=85, a(3)=206, a(4)=472, a(5)=1116, a(6)=2575,
 *            a(7)=6068, a(8)=14096, a(9)=33044, a(10)=76925
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 49, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[10] = {
    48ull, 85ull, 206ull, 472ull, 1116ull,
    2575ull, 6068ull, 14096ull, 33044ull, 76925ull
};

static seq_t *seq_tab = NULL;   /* memoised values */
static idx_t  seq_len = 0;      /* number of terms stored (a(1)..a(seq_len)) */

seq_t a(idx_t n)
{
    /* 1‑based index, n >= 1 */
    if (n == 0) return 0;  /* safety guard */

    /* Initialise table with the first 10 terms if not yet done */
    if (seq_len == 0) {
        size_t alloc = (size_t)((n < 10) ? 10 : n);
        seq_tab = (seq_t *)malloc(alloc * sizeof(seq_t));
        if (!seq_tab) return 0; /* allocation failure */
        for (idx_t i = 0; i < 10; ++i) seq_tab[i] = init_terms[i];
        seq_len = 10;
    }

    /* Return directly for the first 10 terms */
    if (n <= 10) return init_terms[n - 1];

    /* Grow and compute missing terms if necessary */
    if (n > seq_len) {
        size_t newsize = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(seq_tab, newsize * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure, keep old table */
        seq_tab = tmp;

        /* Compute terms seq_len+1 .. n */
        for (idx_t i = seq_len + 1; i <= n; ++i) {
            seq_t t =
                2 * seq_tab[i - 3] +          /* 2*a(i‑2) */
                2 * seq_tab[i - 4] +          /* 2*a(i‑3) */
                11 * seq_tab[i - 5] +         /* 11*a(i‑4) */
                6 * seq_tab[i - 6] +          /* 6*a(i‑5) */
                2 * seq_tab[i - 7] +          /* 2*a(i‑6) */
                6 * seq_tab[i - 8] +          /* 6*a(i‑7) */
                2 * seq_tab[i - 10];          /* 2*a(i‑9) */
            t -= 8 * seq_tab[i - 9];          /* -8*a(i‑8) */
            seq_tab[i - 1] = t;
        }
        seq_len = n;
    }

    return seq_tab[n - 1];
}
