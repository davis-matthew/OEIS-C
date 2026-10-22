/*
 * A389336  Rectangular array, (C(n,k)), read by descending antidiagonals:
 *          C(n,k) = n++k, the concatenation sum of n and k, defined as the
 *          index of the concatenation w(n)w(k) in the sequence S = (w(n)) of
 *          all 01-words in lexicographic order.
 * https://oeis.org/A389336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A389336.
   a(n) = index of w(n)w(k) in the lexicographic list S of all 01‑words,
   where (n,k) is the n‑th point read by descending antidiagonals on the array C(i,j).  */

/* ------------------------------------------------------------------ */
/* Helper: compute floor(log2(v)) for a 64‑bit unsigned integer.      */
static inline idx_t floor_log2_idx(idx_t v)
{
    /* v >= 1; safe for all idx_t values used in this problem. */
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* ------------------------------------------------------------------ */
/* The requested sequence function.                                   */
seq_t a(idx_t n)
{
    /* 1. locate the antidiagonal (s) that contains the n‑th entry
       by solving (s-1)*s/2 >= n.  s starts at 2 because the first
       diagonal contains only the pair (1,1).                           */
    idx_t s = 2;
    while ((s - 1) * s / 2 < n) ++s;

    /* 2. position t of this entry within the diagonal (1‑based).     */
    idx_t prev = (s - 1) * (s - 2) / 2;
    idx_t t = n - prev;

    /* 3. coordinates (i,j) on the array C(i,j).                     */
    idx_t i = t;
    idx_t j = s - t;

    /* 4. lengths of the 01‑words w(i) and w(j).                    */
    idx_t len_i = floor_log2_idx(i + 1);
    idx_t len_j = floor_log2_idx(j + 1);

    /* 5. offsets of w(i) and w(j) within their respective length blocks.
       The first word of length L has index 2^L - 1.                      */
    idx_t start_i = ((idx_t)1 << len_i) - 1;
    idx_t start_j = ((idx_t)1 << len_j) - 1;
    idx_t off_i   = i - start_i;
    idx_t off_j   = j - start_j;

    /* 6. concatenate the two binary strings and compute the index.   */
    idx_t total_len = len_i + len_j;
    idx_t off_concat = (off_i << len_j) | off_j;

    seq_t base    = ((seq_t)1) << total_len;       /* 2^total_len  */
    seq_t result  = base - 1 + (seq_t)off_concat;  /* (2^L - 1) + value  */

    return result;
}
