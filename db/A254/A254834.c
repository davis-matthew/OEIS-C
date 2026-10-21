/*
 * A254834  Number of length 7 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3 or 5.
 * https://oeis.org/A254834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑7 arrays with entries from 1 .. (n+1) *
 *            whose every leading partial sum is divisible by 2, 3, or 5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 31, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Array of residues modulo 30 that are divisible by 2, 3, or 5. */
static const bool ALLOW[30] = {
    true,  /* 0   divisible by all */
    false, /* 1  */
    true,  /* 2  */
    true,  /* 3  */
    true,  /* 4  */
    true,  /* 5  */
    true,  /* 6  */
    false, /* 7  */
    true,  /* 8  */
    true,  /* 9  */
    true,  /*10  */
    false, /*11  */
    true,  /*12  */
    false, /*13  */
    true,  /*14  */
    true,  /*15  */
    true,  /*16  */
    false, /*17  */
    true,  /*18  */
    false, /*19  */
    true,  /*20  */
    true,  /*21  */
    true,  /*22  */
    false, /*23  */
    true,  /*24  */
    true,  /*25  */
    true,  /*26  */
    true,  /*27  */
    true,  /*28  */
    false  /*29  */
};

/* Count the number of integers in [1..m] having each residue modulo 30. */
static void residue_counts(idx_t m, seq_t res[30]) {
    for (int r = 0; r < 30; ++r) {
        idx_t first = (r == 0) ? 30 : (idx_t)r;
        if (first > m) {
            res[r] = (seq_t)0;
        } else {
            /* number of terms: 1 + floor((m - first)/30) */
            seq_t cnt = (seq_t)((m - first) / 30 + 1);
            res[r] = cnt;
        }
    }
}

seq_t a(idx_t n) {
    /* Elements are 1 .. n+1 */
    idx_t m = n + 1;

    seq_t counts[30];
    residue_counts(m, counts);

    seq_t cur[30] = {0};
    seq_t nxt[30];
    cur[0] = (seq_t)1;   /* starting with sum 0 (allowed) */

    /* Perform 7 transitions (array length = 7) */
    for (int step = 0; step < 7; ++step) {
        for (int d = 0; d < 30; ++d) {
            if (!ALLOW[d]) {
                nxt[d] = (seq_t)0;
                continue;
            }
            seq_t sum = (seq_t)0;
            for (int t = 0; t < 30; ++t) {
                if (cur[t] == (seq_t)0) continue;
                int r = d - t;
                if (r < 0) r += 30;
                if (counts[r] != (seq_t)0)
                    sum += cur[t] * counts[r];
            }
            nxt[d] = sum;
        }
        /* Copy nxt into cur for next step */
        for (int i = 0; i < 30; ++i) cur[i] = nxt[i];
    }

    /* Sum over all allowed residues to get total number of arrays */
    seq_t result = (seq_t)0;
    for (int d = 0; d < 30; ++d) {
        if (ALLOW[d])
            result += cur[d];
    }
    return result;
}
