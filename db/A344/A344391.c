/*
 * A344391  T(n, k) = binomial(n - k, k) * factorial(k), for n >= 0 and 0 <= k
 *          <= floor(n/2). Triangle read by rows.
 * https://oeis.org/A344391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS  A344391
   Triangle T(n,k) = binomial(n-k,k) * factorial(k), 0 ≤ k ≤ ⌊n/2⌋
   The sequence a(n) is the flattening of this triangle row by row.
   The function below returns the n‑th element of that flattened sequence. */

#include <stdlib.h>

static seq_t *flat      = NULL;   /* flattened values */
static size_t flat_len  = 0;       /* number of stored entries   */
static size_t flat_cap  = 0;       /* capacity of the flat buffer */

static seq_t *row_cur   = NULL;   /* values of the current row */
static size_t row_len   = 0;       /* number of k's in current row */
static idx_t  row_n     = 0;       /* n of the current row */

/* Ensure that the flattened buffer can hold at least 'need' elements
   (reallocating if necessary). */
static void ensure_flat_capacity(size_t need)
{
    if (need > flat_cap) {
        size_t new_cap = flat_cap ? flat_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        flat = (seq_t*)realloc(flat, new_cap * sizeof(seq_t));
        flat_cap = new_cap;
    }
}

/* Initialise the very first row (n = 0). */
static void init(void)
{
    row_n   = 0;
    row_len = 1;
    row_cur = (seq_t*)malloc(sizeof(seq_t));
    row_cur[0] = 1;          /* T(0,0) = 1 */

    ensure_flat_capacity(1);
    flat[0] = 1;
    flat_len = 1;
}

/* Generate the next triangle row, updating the flattened array. */
static void next_row(void)
{
    idx_t  n       = row_n + 1;
    size_t new_len = n / 2 + 1;           /* floor(n/2)+1 terms in the row */
    seq_t *new_row = (seq_t*)malloc(new_len * sizeof(seq_t));

    new_row[0] = 1;                       /* T(n,0) = 1 */

    for (idx_t k = 1; k < new_len; ++k) {
        seq_t term1 = (k < row_len) ? row_cur[k] : 0;   /* T(n-1,k)   */
        seq_t term2 = 0;     /* contribution from T(n-1,k-1) */

        if (k-1 < row_len) {
#ifdef __cplusplus
            /* For C++ (big‑int class), use normal operators. */
            seq_t t = row_cur[k-1];
            t = t * k * (n - 2*k + 1);
            t = t / (n - k);
            term2 = t;
#else
            /* For C (fixed width integer types), use 128‑bit arithmetic
               to avoid overflow in the intermediate product. */
            term2 = (seq_t)(((__uint128_t)row_cur[k-1] * k *
                             (n - 2*k + 1)) / (n - k));
#endif
        }

        new_row[k] = term1 + term2;
    }

    /* Append the new row to the flattened buffer. */
    for (size_t i = 0; i < new_len; ++i) {
        ensure_flat_capacity(flat_len + 1);
        flat[flat_len++] = new_row[i];
    }

    /* Prepare for the next iteration. */
    free(row_cur);
    row_cur = new_row;
    row_len = new_len;
    row_n   = n;
}

/* Public interface required by the test harness. */
seq_t a(idx_t n)
{
    if (!flat) init();          /* lazy initialisation */

    while (flat_len <= n) {
        next_row();             /* generate whichever rows are missing */
    }
    return flat[n];
}
