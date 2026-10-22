/*
 * A320919  Positive integers k such that binomial(k, 3) is divisible by 6.
 * https://oeis.org/A320919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 570, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n), the n‑th positive integer k such that C(k,3) is divisible by 6.
   The test reduces to: k(k-1)(k-2) must be divisible by 36.
   This implementation simply scans k=1,2,3,… and counts the qualifying values. */

seq_t a(idx_t n)
{
    idx_t    count = 0;   /* how many qualifying k's we've seen */
    for (idx_t k = 1; ; ++k) {
        if (k < 3) {          /* C(k,3)=0 for k=1,2, so always qualifies */
            ++count;
            if (count == n) return (seq_t)k;
            continue;
        }

        seq_t prod = (seq_t)k * (k - 1) * (k - 2);  /* k(k-1)(k-2) */
        if ((prod % 36) == 0) {                   /* divisible by 36? */
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}
