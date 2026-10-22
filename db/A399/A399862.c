/*
 * A399862  Triangle read by rows: T(n,k) = binomial(n-k+1, 2) - [n-k+1 > k] *
 *          binomial(n-2*k+1, 2), k = 1..n, where brackets are Iverson.
 * https://oeis.org/A399862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 3691, u32 n <= 9906, u64 n <= 9906,
 *            u128 n <= 9906, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows:
   T(n,k) = binomial(n-k+1,2) - [n-k+1>k] * binomial(n-2*k+1,2)
   a(n) is the linearization of this triangle (row–major). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; behaviour for n = 0 is irrelevant. */
    if (n == 0) return 0;

    /* Binary search for the row r such that 1 + 2 + … + r >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (tri >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* k is the position within row r, 1‑based */
    seq_t terms_before_seq = ((seq_t)r * (seq_t)(r - 1)) / 2;
    seq_t k_seq = (seq_t)n - terms_before_seq;  /* 1‑based index inside the row */

    seq_t r_seq = r;
    seq_t nmk1 = r_seq - k_seq + 1;             /* n - k + 1 */

    /* First binomial term: binomial(nmk1, 2) = nmk1 * (nmk1-1) / 2 */
    seq_t T1 = nmk1 * (nmk1 - 1) / 2;

    /* Subtract the second term if the Iverson bracket is true */
    seq_t T2 = 0;
    if (nmk1 > k_seq) {
        seq_t n2k1 = r_seq - k_seq * 2 + 1;    /* n - 2k + 1 */
        T2 = n2k1 * (n2k1 - 1) / 2;
    }

    return T1 - T2;
}
