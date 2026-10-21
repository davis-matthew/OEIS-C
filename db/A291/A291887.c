/*
 * A291887  Number of symmetrically unique Dyck paths of semilength n and
 *          height three.
 * https://oeis.org/A291887
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) - 8*a(n-3) + 27*a(n-4) - 10*a(n-5) -
 *            10*a(n-6) + 4*a(n-7) for n >= 10, with initial values: a(3)=1,
 *            a(4)=3, a(5)=11, a(6)=31, a(7)=91, a(8)=250, a(9)=690
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq = NULL;          /* cache of computed terms  */
static idx_t seq_len = 0;          /* number of stored terms (offset 3) */

/* Ensure that values up to index n are computed */
static void ensure_seq(idx_t n)
{
    if (n < 3) return;

    if (seq == NULL) {                /* initialise cache with first 7 terms */
        seq_len = 7;
        seq = (seq_t *)calloc(seq_len, sizeof(seq_t));
        if (seq == NULL) return;     /* allocation failure – return 0s */
        seq[0] = (seq_t)1;   /* a(3) */
        seq[1] = (seq_t)3;   /* a(4) */
        seq[2] = (seq_t)11;  /* a(5) */
        seq[3] = (seq_t)31;  /* a(6) */
        seq[4] = (seq_t)91;  /* a(7) */
        seq[5] = (seq_t)250; /* a(8) */
        seq[6] = (seq_t)690; /* a(9) */
    }

    /* extend cache up to n */
    while ((idx_t)(3 + seq_len) <= n) {
        size_t idx = seq_len;                  /* index of term to compute */
        seq_t pos = 0, neg = 0, term = 0;

        /* positive contributions */
        pos += seq[idx-1] * (seq_t)6;   /* 6*a_{n-1} */
        pos += seq[idx-4] * (seq_t)27;  /* 27*a_{n-4} */
        pos += seq[idx-7] * (seq_t)4;   /* 4*a_{n-7} */

        /* negative contributions */
        neg += seq[idx-2] * (seq_t)9;   /* 9*a_{n-2} */
        neg += seq[idx-3] * (seq_t)8;   /* 8*a_{n-3} */
        neg += seq[idx-5] * (seq_t)10;  /* 10*a_{n-5} */
        neg += seq[idx-6] * (seq_t)10;  /* 10*a_{n-6} */

        term = pos - neg;                /* guaranteed non‑negative */

        seq = (seq_t *)realloc(seq, (seq_len + 1) * sizeof(seq_t));
        seq[seq_len] = term;
        seq_len++;
    }
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;          /* behaviour below offset is unspecified */
    ensure_seq(n);
    return seq[n - 3];
}
