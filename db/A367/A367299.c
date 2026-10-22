/*
 * A367299  Triangular array T(n,k), read by rows: coefficients of strong
 *          divisibility sequence of polynomials p(1,x) = 1, p(2,x) = 2 + 5*x,
 *          p(n,x) = u*p(n-1,x) + v*p(n-2,x) for n >= 3, where u = p(2,x), v =
 *          1 - 2*x - x^2.
 * https://oeis.org/A367299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  
   T(1,0) = 1;  
   T(2,0) = 2,  T(2,1) = 5;  
   T(n,k) = 2*T(n-1,k) + 5*T(n-1,k-1)
            + T(n-2,k) - 2*T(n-2,k-1) - T(n-2,k-2).  

   The sequence a(n) is the coefficients read row‑wise from the
   triangular array T(n,k).  For any n ≥ 1 the function returns T(r,k),
   where r = ⌈(√(8n+1)-1)/2⌉ and k = n - r*(r-1)/2 - 1 (0‑based). */
#include <stdlib.h>
#include <stdint.h>

static seq_t **s_rows = NULL;   /* pointers to each row (0‑based) */
static size_t    s_num  = 0;     /* number of rows currently cached */
static size_t    s_cap  = 0;     /* capacity of s_rows array */

/* Find the 1‑based row number that contains position n (n ≥ 1). */
static size_t row_of(idx_t n)
{
    size_t low  = 1;
    size_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;                 /* double until high satisfies */

    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;   /* 1‑based */
}

/* Ensure that at least `need` rows are present in the cache. */
static void ensure_rows(size_t need)
{
    if (s_num >= need)
        return;

    /* Grow the row pointer array if needed. */
    if (s_cap < need) {
        size_t new_cap = s_cap ? s_cap * 2 : 4;
        if (new_cap < need)
            new_cap = need;
        seq_t **tmp = (seq_t **)realloc(s_rows, new_cap * sizeof(seq_t *));
        if (!tmp)
            abort();          /* allocation failure */
        s_rows = tmp;
        s_cap  = new_cap;
    }

    /* Build new rows one by one. */
    for (; s_num < need; ++s_num) {
        size_t len = s_num + 1;               /* length of this row */
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row)
            abort();                          /* allocation failure */

        if (s_num == 0) {                     /* n = 1 */
            row[0] = (seq_t)1;
        } else if (s_num == 1) {              /* n = 2 */
            row[0] = (seq_t)2;
            row[1] = (seq_t)5;
        } else {
            const seq_t *prev     = s_rows[s_num - 1];   /* T(n-1,·) */
            const seq_t *prevprev = s_rows[s_num - 2];   /* T(n-2,·) */
            size_t prev_len     = s_num;      /* len of prev       */
            size_t prevprev_len = s_num - 1;  /* len of prevprev   */

            for (size_t i = 0; i < len; ++i) {
                seq_t val = (seq_t)0;

                /* 2 * prev[i] */
                if (i < prev_len)
                    val += prev[i] * (seq_t)2;

                /* 5 * prev[i-1] */
                if (i >= 1 && i - 1 < prev_len)
                    val += prev[i - 1] * (seq_t)5;

                /* prevprev[i] */
                if (i < prevprev_len)
                    val += prevprev[i];

                /* -2 * prevprev[i-1] */
                if (i >= 1 && i - 1 < prevprev_len)
                    val -= prevprev[i - 1] * (seq_t)2;

                /* - prevprev[i-2] */
                if (i >= 2 && i - 2 < prevprev_len)
                    val -= prevprev[i - 2];

                row[i] = val;
            }
        }
        s_rows[s_num] = row;
    }
}

/* Main function demanded by the build. */
seq_t a(idx_t n)
{
    if (n < 1)          /* defensive; the sequence is defined for n ≥ 1 */
        return (seq_t)0;

    size_t r = row_of(n);          /* 1‑based row number */
    size_t r_idx = r - 1;          /* 0‑based index into s_rows */

    ensure_rows(r_idx + 1);        /* make sure this row exists */

    /* Number of terms that come before this row. */
    idx_t prev_total = (idx_t)(((unsigned __int128)(r - 1) * (unsigned __int128)r) / 2);

    /* Column index within the row (0‑based). */
    size_t k = (size_t)(n - prev_total - 1);

    return s_rows[r_idx][k];
}
