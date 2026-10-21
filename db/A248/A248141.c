/*
 * A248141  Table read by rows: n-th row contains all subsets of consecutive
 *          numbers of 1..n.
 * https://oeis.org/A248141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 8855, u32 n <= 8855, u64 n <= 8855,
 *            u128 n <= 8855, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Table read by rows: n‑th row lists all consecutive subsets of 1..n,
   each subset written as its elements in order. This function returns
   the n‑th element of that table, with a(1)=1. */

static inline seq_t cumulative_terms(idx_t r)
/* cumulative number of terms up to row r: C(r+3,4) = r(r+1)(r+2)(r+3)/24 */
{
    if (r == 0) return 0;
    return ((seq_t)(r + 3) * (r + 2) * (r + 1) * r) / (seq_t)24;
}

static inline seq_t row_length(idx_t r)
/* number of terms in row r: C(r+2,3) = r(r+1)(r+2)/6 */
{
    return ((seq_t)r * (r + 1) * (r + 2)) / (seq_t)6;
}

seq_t a(idx_t n)
{
    /* n is 1‑based.  For n==0 the behaviour is undefined, but we return 0. */
    if (n == 0) return 0;

    /* Find the smallest r such that cumulative_terms(r) >= n */
    idx_t low = 1, high = 1;
    while (cumulative_terms(high) < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (cumulative_terms(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    seq_t cum_prev = cumulative_terms(r - 1);
    seq_t offset = (seq_t)n - cum_prev - 1;  /* 0‑based position within row r */

    /* Find the subset length L that contains the offset */
    seq_t rem = offset;
    for (idx_t L = 1; L <= r; ++L) {
        seq_t group = (seq_t)(r - L + 1) * L;  /* terms contributed by length L */
        if (rem < group) {
            seq_t subset_index = rem / L;
            seq_t start = (seq_t)1 + subset_index;
            seq_t pos = rem % (seq_t)L;
            return start + pos;
        }
        rem -= group;
    }

    return 0;  /* should never be reached */
}
