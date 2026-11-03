/*
 * A343558  Irregular triangle read by rows: the n-th row gives the row
 *          indices of the consecutive elements of the spiral of the n X n
 *          matrix defined in A126224.
 * https://oeis.org/A343558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9455, u16 n <= 9455, u32 n <= 9455, u64 n <= 9455,
 *            u128 n <= 9455, big any n
 * Validated: standard, pass, 107 terms through n = 8047 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: return the row index (1‑based) of the pos‑th element
   in the clockwise spiral traversal of an k×k matrix. */
static idx_t get_row_index(idx_t k, idx_t pos)
{
    /* For k = 1 the only element is in row 1. */
    if (k == 1)
        return 1;

    idx_t top    = 1;
    idx_t bottom = k;
    idx_t left   = 1;
    idx_t right  = k;
    idx_t current = 0;   /* number of cells already processed */

    while (1)
    {
        /* top row : left → right */
        idx_t len = right - left + 1;
        if (current + len >= pos)
            return top;
        current += len;
        ++top;
        if (top > bottom)
            break;

        /* right column : top → bottom */
        len = bottom - top + 1;
        if (current + len >= pos) {
            idx_t offset = pos - current - 1;   /* zero based */
            return top + offset;
        }
        current += len;
        --right;
        if (left > right)
            break;

        /* bottom row : right → left */
        len = right - left + 1;
        if (current + len >= pos)
            return bottom;
        current += len;
        --bottom;
        if (top > bottom)
            break;

        /* left column : bottom → top */
        len = bottom - top + 1;
        if (current + len >= pos) {
            idx_t offset = pos - current - 1;
            return bottom - offset;
        }
        current += len;
        ++left;
    }

    /* Should never be reached for valid inputs. */
    return 0;
}
/* Computes a(n) as the row index of the n-th element in the
   concatenated rows of spirals of size 1×1, 2×2, 3×3, …,
   i.e. the irregular triangle read by rows (OEIS A343558). */
 

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* safety, not used for official offset 1 */

    /* Find the minimal k such that the cumulative number of cells
       up to size k is ≥ n.  The cumulative is sum_{i=1}^{k-1} i^2.
       Incrementally add squares to avoid overflow. */
    idx_t cumulative = 0;    /* sum of squares up to k-1 */
    idx_t k = 0;

    while (1) {
        idx_t k_next = k + 1;
        idx_t sq = k_next * k_next;
        if (cumulative + sq >= n) {
            k = k_next;
            break;
        }
        cumulative += sq;
        k = k_next;
    }

    idx_t pos = n - cumulative;   /* 1‑based position within the k×k spiral */
    idx_t row = get_row_index(k, pos);
    return (seq_t)row;
}
