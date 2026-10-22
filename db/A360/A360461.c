/*
 * A360461  T(n,k) is the sum of all the k-th smallest divisors of all
 *          positive integers <= n. Irregular triangle read by rows (n>=1,
 *          k>=1).
 * https://oeis.org/A360461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(n) = sum of the k‑th smallest divisors of all integers ≤ m,
 *            where the triangle is read row by row. The function returns the
 *            flattened triangle. It builds rows on demand, memoising the
 *            prefix of the triangle so that any n ≥ 1 can be queried in
 *            O(number of rows)
 * Range:     u8 n <= 190, u16 n <= 9174, u32 n <= 9174, u64 n <= 9174,
 *            u128 n <= 9174, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

/* Fixed maximum capacities: enough for n up to ~10 000.  */
#define MAX_FLAT_CAP 200000   /* total number of values stored */
#define MAX_SUMS_CAP 200      /* maximum number of columns in a row */

static seq_t flat[MAX_FLAT_CAP];
static size_t flat_len = 0;          /* how many values are filled */
static seq_t sums[MAX_SUMS_CAP];
static size_t sums_len = 0;          /* current number of columns in sums */
static idx_t current_row = 0;        /* last row that has been processed */

/* ext_row builds the next row of the triangle (for integer current_row+1). */
static void ext_row(void)
{
    /* The next integer to process: */
    idx_t m = ++current_row;

    /* Temporary storage for the divisors of m (they fit in 16 bits).  */
    unsigned int divs[256];
    size_t divs_cnt = 0;
    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            divs[divs_cnt++] = (unsigned int)d;
            if (d != m / d) {
                divs[divs_cnt++] = (unsigned int)(m / d);
            }
        }
    }

    /* Small insertion sort – the array is tiny.  */
    for (size_t i = 0; i < divs_cnt; ++i) {
        for (size_t j = i + 1; j < divs_cnt; ++j) {
            if (divs[j] < divs[i]) {
                unsigned int tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }
        }
    }

    /* Grow the sums array if this row has more columns.  */
    if (divs_cnt > sums_len) {
        /* Initialise the new columns to zero.  */
        for (size_t i = sums_len; i < divs_cnt; ++i) {
            sums[i] = (seq_t)0;
        }
        sums_len = divs_cnt;
        assert(sums_len <= MAX_SUMS_CAP);
    }

    /* Add this integer's divisors to the running sums.  */
    for (size_t i = 0; i < divs_cnt; ++i) {
        sums[i] += (seq_t)divs[i];
    }

    /* Append the current row (all active columns) to the flattened array.  */
    assert(flat_len + sums_len <= MAX_FLAT_CAP);
    for (size_t i = 0; i < sums_len; ++i) {
        flat[flat_len++] = sums[i];
    }
}

/* Public sequence function – returns the a(n) value for n ≥ 1.  */
seq_t a(idx_t n)
{
    /* Produce rows until we have at least n entries.  */
    while (flat_len < (size_t)n) {
        ext_row();
    }
    return flat[n - 1];
}
