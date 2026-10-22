/*
 * A327728  Number of unlabeled multigraphs with loops allowed and n edges
 *          covering three vertices.
 * https://oeis.org/A327728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 31, u32 n <= 311, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A327728: Number of unlabeled multigraphs with loops allowed
   and n edges covering three vertices.
   Recurrence (for n > 12):
      a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - 3*a(n-4)
             + 6*a(n-6) - 3*a(n-8) - 2*a(n-9) + a(n-10)
             + 2*a(n-11) - a(n-12)          */
/* Base values a(1) … a(12) are taken from the OEIS entry. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For any larger n the
       following memoised recurrence is used. */
    if (n == 0) {
        return (seq_t)0;   /* 0 is never requested, but keeps type safety */
    }

    /* Hard‑coded initial terms.  base[0] is set to 0 for convenience. */
    static const seq_t base[13] = {
        0,     /* 0 – unused */
        0,     /* a(1) */
        2,     /* a(2) */
        8,     /* a(3) */
        19,    /* a(4) */
        40,    /* a(5) */
        77,    /* a(6) */
        132,   /* a(7) */
        217,   /* a(8) */
        340,   /* a(9) */
        510,   /* a(10) */
        742,   /* a(11) */
        1054   /* a(12) */
    };

    /* Memoisation storage */
    static seq_t *buf = NULL;      /* array of computed terms */
    static size_t buf_capacity = 0;/* allocated size of buf */
    static idx_t max_computed = 0;/* highest n for which buf[n] is valid */

    /* Expand buffer if necessary (buf[0] is always valid for safety). */
    if ((size_t)(n + 1) > buf_capacity) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out‑of‑memory – safe exit */
        buf = tmp;
        buf_capacity = new_cap;
    }

    /* If value already present, return it immediately. */
    if (n <= max_computed) {
        return buf[n];
    }

    /* Fill in missing base terms up to 12. */
    if (max_computed < 12) {
        idx_t start = max_computed + 1;
        if (start < 1) start = 1;
        for (idx_t i = start; i <= 12 && i <= n; ++i) {
            buf[i] = base[i];
        }
        max_computed = (n < 12) ? n : 12;
        if (n <= 12) return buf[n];
        /* Next index to compute is now 13. */
    }

    /* Compute values for n ≥ 13 using the given recurrence. */
    for (idx_t i = (idx_t)13; i <= n; ++i) {
        seq_t sum_pos = buf[i - 1];
        sum_pos += sum_pos;               /* 2*a(i-1) */
        sum_pos += buf[i - 2];             /* +a(i-2) */
        seq_t tmp = buf[i - 6];
        tmp *= 6;                          /* +6*a(i-6) */
        sum_pos += tmp;
        sum_pos += buf[i - 10];            /* +a(i-10) */
        tmp = buf[i - 11];
        tmp *= 2;                          /* +2*a(i-11) */
        sum_pos += tmp;

        seq_t sum_neg = buf[i - 3];
        sum_neg += sum_neg;               /* 2*a(i-3) */
        tmp = buf[i - 4];
        tmp *= 3;                          /* 3*a(i-4) */
        sum_neg += tmp;
        tmp = buf[i - 8];
        tmp *= 3;                          /* 3*a(i-8) */
        sum_neg += tmp;
        tmp = buf[i - 9];
        tmp *= 2;                          /* 2*a(i-9) */
        sum_neg += tmp;
        sum_neg += buf[i - 12];            /* +a(i-12) */

        buf[i] = sum_pos - sum_neg;
    }

    max_computed = n;
    return buf[n];
}
