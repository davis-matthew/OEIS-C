/*
 * A264758  An eventually quasi-cubic solution to Hofstadter's Q recurrence.
 * https://oeis.org/A264758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 320, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1) = 7, a(2) = 0.
 * For n ≥ 3:
 *   Let r = n mod 9.
 *   r == 0:   a(n) = n
 *   r == 1:   a(n) = 9
 *   r == 2:   a(n) = 3
 *   r == 3:   a(n) = (9*k*k + 9*k)/2 + 5,     where k = (n-3)/9
 *   r == 4:   a(n) = 9
 *   r == 5:   a(n) = 3
 *   r == 6:   a(n) = (3*k*k*k + 9*k*k)/2 + 8*k + 8,  where k = (n-6)/9
 *   r == 7:   a(n) = 9
 *   r == 8:   a(n) = 2
 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)0;

    idx_t r = n % 9;
    if (r == 0) return (seq_t)n;
    if (r == 1) return (seq_t)9;
    if (r == 2) return (seq_t)3;
    if (r == 4) return (seq_t)9;
    if (r == 5) return (seq_t)3;
    if (r == 7) return (seq_t)9;
    if (r == 8) return (seq_t)2;

    idx_t k;
    if (r == 3) {
        k = (n - 3) / 9;
        seq_t kk = (seq_t)k;
        return (9 * kk * kk + 9 * kk) / 2 + (seq_t)5;
    } else { /* r == 6 */
        k = (n - 6) / 9;
        seq_t kk = (seq_t)k;
        return (3 * kk * kk * kk + 9 * kk * kk) / 2 + 8 * kk + (seq_t)8;
    }
}
