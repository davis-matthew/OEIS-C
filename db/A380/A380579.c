/*
 * A380579  Triangle read by rows in which row n lists n successive integers
 *          in descending order starting with the n-th positive integer not
 *          divisible by 3, with n >= 1 and 1 <= k <= n.
 * https://oeis.org/A380579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = A001651(row) - (pos-1) where 'row' is the unique r with
 *            r(r-1)/2 < n ≤ r(r+1)/2 and 'pos' = n - r(r-1)/2. A001651(r)
 *            gives the r‑th positive integer not divisible by 3. The triangle
 *            row r then contains the numbers A001651(r), A001651(r)-1, …,
 *            A001651(r)-r+1. The sequence a(n) is obtained b...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Empty or zero index is outside the offset – return 0 (unused). */
    if (n == 0) return 0;

    /* Find the row 'r' such that T(r-1) < n ≤ T(r) where
       T(r) = r(r+1)/2 is the r‑th triangular number. */
    idx_t r = 0;
    idx_t tri = 0;          /* T(r) */
    while (tri < n) {
        ++r;
        tri += r;          /* tri = T(r) after increment */
    }
    idx_t ttri = tri;          /* T(r) */
    idx_t prev_tri = ttri - r; /* T(r-1) */
    idx_t k = n - prev_tri;    /* position within the row (1‑based) */

    /* Compute A001651(r) – the r‑th positive integer not divisible by 3.
       For r = 1,2,3,4,5,… the sequence is 1,2,4,5,7,8,10,11,….
       The following closed form works for all r: */
    idx_t t = r - 1;                       /* r-1 */
    seq_t start = 3 * (seq_t)(t / 2) +     /* 3*floor((r-1)/2) */
                  ( (r & 1) ? 1 : 2 );     /* odd => +1, even => +2 */

    /* The k‑th element of the row (k ∈ [1, r]) is start-(k-1). */
    seq_t result = start - (seq_t)(k - 1);
    return result;
}
