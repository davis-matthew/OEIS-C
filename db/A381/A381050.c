/*
 * A381050  Triangle T(n,k) read by rows, where row n is a permutation of the
 *          numbers 1 through n, such that if a deck of n cards is prepared in
 *          this order, and down-under-down dealing is used, then the
 *          resulting cards will be dealt in increasing order.
 * https://oeis.org/A381050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A381050: Triangle permutation that produces ascending order when a deck of
 * cards is dealt by the "down‑under‑down" pattern.  For a given overall
 * index n (starting with 1), this function returns the value a(n) in that
 * flattened triangle.
 */

#include <stdlib.h>   /* for malloc, free */

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* 1. Determine row r and position k within that row. */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) {          /* t(r) = r*(r+1)/2, grow to >= n */
        ++r;
        sum += r;
    }
    /* r is now the row number, sum == r*(r+1)/2 */
    idx_t prev = sum - r;      /* sum of previous rows = r*(r-1)/2 */
    idx_t k = n - prev;        /* 1‑based column within the row */

    /* 2. Build the permutation for row r using the reverse‑simulation
     * of the dealing process, which works in O(r) time. */
    idx_t *buf = (idx_t *)malloc((size_t)r * sizeof(idx_t));
    if (buf == NULL) return (seq_t)0;  /* safety on allocation failure */

    idx_t cap = r;          /* circular buffer capacity */
    idx_t start = 0, end = 0;   /* deque indices (end points after back) */
    size_t len = 0;

    /* If r is odd there is a final round that deals only one card (the
     * largest value).  Reverse that round first. */
    if ((r & 1U) != 0) {
        start = (start == 0 ? cap - 1 : start - 1);
        buf[start] = (idx_t)r;    /* push_front r */
        ++len;
    }

    /* Number of full rounds (each dealing two cards). */
    idx_t rounds = r >> 1;   /* r / 2 */
    for (idx_t i = rounds; i >= 1; --i) {
        idx_t a = 2 * i - 1;
        idx_t b = a + 1;

        idx_t m = 0;
        int have_m = 0;
        if (len > 0) {
            /* pop_back for the moved card from the previous round */
            size_t backIdx = (end == 0 ? cap - 1 : end - 1);
            m = buf[backIdx];
            end = backIdx;
            --len;
            have_m = 1;
        }

        /* push_front b */
        start = (start == 0 ? cap - 1 : start - 1);
        buf[start] = b;
        ++len;

        /* push_front m (the card that was moved to the back) */
        if (have_m) {
            start = (start == 0 ? cap - 1 : start - 1);
            buf[start] = m;
            ++len;
        }

        /* push_front a */
        start = (start == 0 ? cap - 1 : start - 1);
        buf[start] = a;
        ++len;
    }

    /* 3. Retrieve the k‑th element (1‑based) from the built permutation. */
    idx_t pos = (start + (k - 1)) % cap;
    seq_t result = (seq_t)buf[pos];

    free(buf);
    return result;
}
