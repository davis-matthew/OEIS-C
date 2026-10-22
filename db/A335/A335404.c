/*
 * A335404  Numbers k such that the k-th composition in standard order
 *          (A066099) has the same product as sum.
 * https://oeis.org/A335404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A335404: a(n) is the n-th integer k such that the product of the parts of the
 * k‑th composition in standard order equals the sum of those parts. */

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;                     /* current integer to test */

    while (1) {
        /* Compute sum of the composition: number of bits in k. */
        idx_t sum = 0;
        idx_t tmp = k;
        while (tmp) {
            tmp >>= 1;
            sum++;
        }

        /* Compute product of the composition parts. */
        seq_t prod = 1;
        idx_t prev = 0;
        tmp = k;
        idx_t bitpos = 0;
        while (tmp) {
            if (tmp & 1) {
                idx_t part = (bitpos + 1) - prev;
                prod *= part;
                prev = bitpos + 1;
            }
            bitpos++;
            tmp >>= 1;
        }

        /* Check condition product == sum. */
        if (prod == (seq_t)sum) {
            count++;
            if (count == n)
                return (seq_t)k;
        }

        k++;
    }
}
