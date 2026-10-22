/*
 * A340958  Concatenated layouts of page numbers as in a printed bound booklet
 *          with 2 pages per side with 0's representing blanks.
 * https://oeis.org/A340958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenated page layouts for a bound booklet (A340958)
   Each n gives the n-th number in the flattened sequence of
   page numbers produced while increasing the page count N from 1 upward.
   For a given N, let S = ceil(N/4) (the number of sheets).
   For each sheet i (0‑based, 0 ≤ i < S) the four numbers are:
       left_out  = 4*S − 2*i
       right_out = 2*i + 1
       left_in   = 2*i + 2
       right_in  = 4*S − 2*i − 1
   Numbers larger than N are treated as 0 (blanks).  The four values for
   each sheet are output in the order above, so the sequence consists of
   all those values for N = 1, 2, 3, … in order. */
seq_t a(idx_t n)
{
    /* First locate the page count N that contains the n-th value. */
    idx_t N = 1;            /* current page count */
    idx_t remaining = n;    /* still needed index (1‑based) */

    while (1) {
        /* number of sheets for this N */
        idx_t sheets = (N + 3) / 4;          /* ceil(N/4) */
        idx_t thisTerms = 4 * sheets;        /* 4 numbers per sheet */

        if (remaining > thisTerms) {
            remaining -= thisTerms;
            ++N;
        } else {
            break;
        }
    }

    /* Now N is fixed.  Determine the sheet and position within the sheet. */
    idx_t sheets    = (N + 3) / 4;
    idx_t offset    = remaining - 1;         /* 0‑based offset in this block */
    idx_t sheetIndex = offset / 4;           /* 0‑based sheet index */
    idx_t pos        = offset % 4;           /* position 0: left_out ... 3: right_in */

    /* Compute the four page numbers for this sheet. */
    idx_t left_out  = 4 * sheets - 2 * sheetIndex;
    idx_t right_out = 2 * sheetIndex + 1;
    idx_t left_in   = 2 * sheetIndex + 2;
    idx_t right_in  = 4 * sheets - 2 * sheetIndex - 1;

    /* Return the value at the requested position, using 0 for blanks. */
    switch (pos) {
        case 0: return (left_out  <= N) ? (seq_t)left_out  : 0;
        case 1: return (right_out <= N) ? (seq_t)right_out : 0;
        case 2: return (left_in   <= N) ? (seq_t)left_in   : 0;
        default:/* case 3 */
            return (right_in  <= N) ? (seq_t)right_in  : 0;
    }
}
