/*
 * A278711  Triangle T read by rows: T(n, m), for n >= 2, and m=1, 2, ...,
 *          n-1, equals the positive integer solution x of y^2 = x^3 - A(n,
 *          m)^2*x with the area A(n, m) = A249869(n, m) of the primitive
 *          Pythagorean triangle characterized by (n, m) or 0 if no such
 *          triangle exists.
 * https://oeis.org/A278711
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A278711
 *  Triangle T read by rows:  T(n, m) = (n² - m²) * n²
 *  for n > m ≥ 1, gcd(n, m) = 1 and n + m odd; otherwise 0.
 *  The linear sequence a(n) is obtained by reading the triangle row‑wise
 *  starting with offset 2:  a(2) = T(2,1), a(3) = T(3,1), a(4) = T(3,2), …
 *
 *  The function below implements this definition directly.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2. For smaller indices just return 0. */
    if (n < 2)
        return 0;

    /* Convert to a 0‑based index to simplify the row/column calculation. */
    idx_t idx = n - 2;          /* indices: 0 → (2,1), 1 → (3,1), 2 → (3,2), … */

    /* Find the row r such that the cumulative number of terms up to this
     * row exceeds idx.  The number of terms in row r is (r‑1).  */
    idx_t r = 2;                /* start with the first row that has terms */
    idx_t cumulative = 0;       /* total terms in all previous rows */

    while (cumulative + (r - 1) <= idx) {
        cumulative += r - 1;
        r++;
    }

    /* Column m within the chosen row.  Columns are 1‑based. */
    idx_t m = idx - cumulative + 1;

    /* Validate the parameters – 0 is returned if the row or column
     * lies outside the valid bounds.  */
    if (m < 1 || m >= r)
        return 0;

    /* Apply the gcd and parity conditions. */
    if (gcd((seq_t)r, (seq_t)m) != (seq_t)1)
        return 0;
    if (((r + m) & 1) == 0)   /* n + m is even → not primitive */
        return 0;

    /* Compute (n² - m²) * n².  All arithmetic is done in seq_t to
     * support arbitrary‑precision builds. */
    seq_t rr = (seq_t)r;
    seq_t mm = (seq_t)m;
    seq_t r2 = rr * rr;         /* r²  */
    seq_t diff = r2 - mm * mm;  /* r² – m²  */
    return diff * r2;
}
