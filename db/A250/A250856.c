/*
 * A250856  Number of (4+1) X (n+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1..7) = {12311, 63631, 223933, 626416, 1499679, 3204951,
 *            6279401} a(n) = 7·a(n‑1) – 21·a(n‑2) + 35·a(n‑3) – 35·a(n‑4) +
 *            21·a(n‑5) – 7·a(n‑6) + a(n‑7) for n > 7
 * Range:     u8 none, u16 n <= 2, u32 n <= 25, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence memoisation ----------------------------------------------------- */
static seq_t *memo = NULL;
static idx_t memo_size = 0;  /* largest n for which memo[n] is valid */

/* The five initial terms (index 0 unused) ----------------------------------- */
static const seq_t init[8] = {
    0,          /* dummy for index 0 */
    12311,      /* a(1) */
    63631,      /* a(2) */
    223933,     /* a(3) */
    626416,     /* a(4) */
    1499679,    /* a(5) */
    3204951,    /* a(6) */
    6279401,    /* a(7) */
};

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) {            /* offset 1, but keep behaviour sensible */
        return 0;
    }

    /* Grow the memoisation table if necessary --------------------------------- */
    if (n > memo_size) {
        idx_t new_size = n + 1;
        seq_t *new_mem = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!new_mem) { /* allocation failed; terminate */
            exit(1);
        }
        memo = new_mem;
        /* initialise new entries */
        for (idx_t i = memo_size + 1; i <= n; ++i) {
            if (i <= 7) {
                memo[i] = init[i];
            } else {
                /* use signed 128‑bit arithmetic for the intermediate sum */
                __int128 sum = 0;
                sum += (__int128)memo[i-1] * 7;
                sum -= (__int128)memo[i-2] * 21;
                sum += (__int128)memo[i-3] * 35;
                sum -= (__int128)memo[i-4] * 35;
                sum += (__int128)memo[i-5] * 21;
                sum -= (__int128)memo[i-6] * 7;
                sum += (__int128)memo[i-7];
                memo[i] = (seq_t)sum;
            }
        }
        memo_size = n;
    }

    return memo[n];
}
