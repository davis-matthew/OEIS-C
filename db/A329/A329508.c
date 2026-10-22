/*
 * A329508  Array read by upward antidiagonals: row n = coordination sequence
 *          for cylinder formed by rolling up a strip of width n hexagons cut
 *          from the hexagonal grid by cuts parallel to grid lines.
 * https://oeis.org/A329508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  OEIS A329508 – Array read by upward antidiagonals in the coordination
 *  sequences of hexagonal cylinders.
 *
 *  For a given n (n ≥ 1) let d be the smallest integer with  
 *      d(d+1)/2 ≥ n.  Then
 *      tPrev = (d-1)d/2          // elements before diagonal d
 *      offset = n – tPrev – 1    // 0‑based index inside diagonal d
 *      row    = d – offset
 *      col    = offset + 1
 *
 *  The value a(n) is the coefficient of x^(col‑1) in the generating
 *  function of row  “row”:
 *
 *      G(row) = (1/(1-x))*(1 + 2*x + 3*x^2*(1-x^(row-2))/(1-x)
 *                         + 2*x^row - x^(row+2)*(1-x^(row-1))/(1-x))
 *
 *  The coefficient can be computed directly without expanding the
 *  rational functions by the closed‑form formula derived from the
 *  partial fractions:
 *
 *          val = 1
 *          val += 2   if col>1
 *          val += 2   if col-1 ≥ row
 *          val += 3*(col-2)                if col>2
 *          val -= 3*(col-1-row)            if col-1 ≥ row
 *          val -= (col-2-row)              if col-1 ≥ row+2
 *          val += (col-1-2*row)            if col-1 ≥ 2*row+1
 *
 *  That formula gives the exact row/col value for all rows ≥1.
 */

static seq_t row_col_value(idx_t row, idx_t col)
{
    /* col and row are 1‑based; k = col-1 is the index in the generating
       function series. */
    idx_t k = col - 1;
    seq_t val = 0;

    /* Contributions of the (1 + 2x + 2x^row) terms. */
    val += 1;                     /* coefficient of x^0 */
    if (k >= 1) val += 2;          /* coefficient of x^1 (from 2x) */
    if (k >= row) val += 2;        /* coefficient of x^row (from 2x^row) */

    /* Coefficients coming from the terms with (1‑x^(row-2))/(1‑x)
       and (1‑x^(row-1))/(1‑x). */
    if (k >= 2)
        val += (seq_t)3 * (seq_t)(k - 1);                  /* 3*(k-1)   */
    if (k >= row)
        val -= (seq_t)3 * (seq_t)(k - row + 1);            /* –3*(k-row+1) */
    if (k >= row + 2)
        val -= (seq_t)1 * (seq_t)(k - row - 1);            /* –1*(k-row-1) */
    if (k >= 2 * row + 1)
        val += (seq_t)1 * (seq_t)(k - 2 * row);            /* +1*(k-2*row) */

    return val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; undefined for n==0 */

    /* Find the diagonal number d such that d(d+1)/2 ≥ n. */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n) ++d;

    /* Elements preceding diagonal d. */
    idx_t tPrev = (d - 1) * d / 2;

    /* Position inside the diagonal (0‑based). */
    idx_t offset = n - tPrev - 1;

    /* Convert to row and column of the 2‑D array. */
    idx_t row = d - offset;      /* 1‑based */
    idx_t col = offset + 1;      /* 1‑based */

    return row_col_value(row, col);
}
