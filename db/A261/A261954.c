/*
 * A261954  Start with a single equilateral triangle for n=0; for the odd n-th
 *          generation add a triangle at each expandable side of the triangles
 *          of the (n-1)-th generation (this is the "side to side" version);
 *          for the even n-th generation use the "side to vertex" version;
 *          a(n) is the number of triangles add...
 * https://oeis.org/A261954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A261954
   a(0) = 1, a(1) = 3;
   even n >= 2: a(n) = 9*(n/2) - 6
   odd n >= 3 : a(n) = a(n-2) + 9 if n%4==1, else a(n-2)+3
   This implementation expands the recurrence into closed‑form
   formulas that run in O(1) time.
*/
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 3;

    if ((n & 1) == 0) {               /* even n */
        return ((seq_t)(n / 2)) * 9 - 6;
    } else {                           /* odd n */
        idx_t k = (n - 1) / 2;          /* k = (n-1)/2  */
        seq_t even_part = ((seq_t)(k / 2)) * 9;          /* 9 * floor(k/2) */
        seq_t odd_part  = ((seq_t)((k + 1) / 2)) * 3;   /* 3 * ceil(k/2)  */
        return 3 + even_part + odd_part;
    }
}
