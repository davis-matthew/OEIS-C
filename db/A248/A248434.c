/*
 * A248434  Number of length three 0..n arrays with the sum of two elements
 *          equal to twice the third.
 * https://oeis.org/A248434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*floor((n+1)/2)*floor(n/2) + n + 1
 * Range:     u8 n <= 12, u16 n <= 208, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of length‑3 arrays of integers 0..n that contain their arithmetic mean */

seq_t a(idx_t n)
{
    /* Compute floor((n+1)/2) without overflow */
    idx_t half1 = (n >> 1) + (n & 1); /* (n+1)/2 */
    idx_t half2 = n >> 1;             /* n/2 */

    seq_t prod = (seq_t)half1 * (seq_t)half2; /* product in seq_t to avoid truncation */
    prod *= 6;                               /* multiply by 6 */

    return prod + (seq_t)n + 1;              /* add n + 1 */
}
