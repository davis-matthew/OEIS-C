/*
 * A398995  The binomial matrix of the Pell numbers. Square array read by
 *          ascending antidiagonals.
 * https://oeis.org/A398995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A398995 – Binomial matrix of the Pell numbers a(n) =
 *            Sum_{m=0..d} binomial(d,m) * s^(d-m) * P(p+m) where d = |row -
 *            col| (distance to diagonal) p = min(row, col) (starting row or
 *            column) s = 1 if col <= row (below or on diagonal) -1 if col >
 *            row (above diagonal) The array is read by ascending ant...
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Compile as C11 or C++17

#include <stdlib.h>

static seq_t *pell = NULL;          /* stored Pell numbers P(0), P(1), ... */
static size_t pell_len = 0;         /* number of stored P(k) */
static size_t pell_cap = 0;         /* allocated capacity */

/* Ensure selected Pell numbers are computed up to index idx (inclusive). */
static void ensure_pell(idx_t idx)
{
    if (pell == NULL) {
        pell_cap = 4;
        pell = (seq_t *)malloc(pell_cap * sizeof(seq_t));   /* cast for C++ */
        if (!pell) exit(1);
        pell[0] = (seq_t)1;         /* P(0) = 1 */
        pell[1] = (seq_t)2;         /* P(1) = 2 */
        pell_len = 2;
    }

    if ((size_t)idx + 1 <= pell_len) return;   /* already present */

    /* Grow storage if necessary */
    size_t needed = (size_t)idx + 1;
    if (needed > pell_cap) {
        size_t new_cap = pell_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(pell, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        pell = tmp;
        pell_cap = new_cap;
    }

    /* Compute missing Pell numbers */
    for (size_t i = pell_len; i <= idx; ++i) {
        /* P(i) = 2*P(i-1) + P(i-2) */
        pell[i] = 2 * pell[i - 1] + pell[i - 2];
    }
    pell_len = idx + 1;
}

seq_t a(idx_t n)
{
    /* Locate the antidiagonal t such that n lies on it.
       Antidiagonals are indexed 0,1,2,..., each containing t+1 terms.
       The cumulative count up to diagonal t-1 is t*(t+1)/2. */
    idx_t t = 0;
    while ((t + 1) * (t + 2) / 2 <= n) t++;

    idx_t prev_count = t * (t + 1) / 2;
    idx_t offset    = n - prev_count;
    idx_t row = t - offset;   /* 0‑based row index on the diagonal */
    idx_t col = offset;       /* 0‑based column index on the diagonal */

    /* Distance to the main diagonal and start index */
    idx_t d = (row >= col) ? (row - col) : (col - row);
    idx_t p = (row <= col) ? row : col;
    int s_sign = (col <= row) ? 1 : -1;

    /* Ensure required Pell numbers are available */
    ensure_pell(p + d);

    seq_t sum = 0;
    seq_t bin = 1;  /* binomial coefficient C(d,0) */

    for (idx_t m = 0; m <= d; ++m) {
        seq_t pell_val = pell[p + m];
        seq_t term = bin * pell_val;        /* binomial * Pell */

        /* Determine sign: s^(d-m) */
        int mul_sign = 1;
        if (s_sign == -1) {
            if ((d - m) & 1) mul_sign = -1;  /* odd exponent -> negative */
        }
        if (mul_sign > 0)
            sum += term;
        else
            sum -= term;

        /* Update binomial coefficient for next m */
        if (m < d) {
            /* C(d,m+1) = C(d,m) * (d-m) / (m+1) */
            bin = bin * (seq_t)(d - m) / (seq_t)(m + 1);
        }
    }

    return sum;
}
