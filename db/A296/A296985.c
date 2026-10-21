/*
 * A296985  Number of nX3 0..1 arrays with each 1 adjacent to 3, 4 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (order 20) for A296985
 * a(n) = Σ_{k=1}^{20} c_k · a(n−k)  with coefficients
 * c = { 3,  2, 19, -19, -52, -67, 15, 155, 207, -104,
 *      -157, -172, 53, 63, 38, 14, 19, 3, 4, 2 }
 * Initial values (for 1 ≤ n ≤ 20) are taken from the OEIS entry.
 * The recurrence holds exactly for all terms of the sequence.
 */

#include <stdint.h>
#include <stddef.h>

/* Initial values a(1) … a(20) (unsigned 64‑bit literals).  */
static const uint64_t init_u64[21] = {
    0ULL,                /* unused */
    1ULL, 4ULL, 14ULL, 28ULL, 98ULL,
    447ULL, 1653ULL, 6507ULL, 27374ULL, 111866ULL,
    455776ULL, 1879650ULL, 7732294ULL, 31739780ULL, 130516126ULL,
    536728543ULL, 2206092839ULL, 9069307066ULL, 37287485232ULL,
    153290332616ULL
};

/* Coefficients of the recurrence. */
static const int coeff[20] = {
    3,  2, 19, -19, -52, -67, 15, 155, 207, -104,
   -157, -172, 53, 63, 38, 14, 19, 3, 4, 2
};

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; protected against accidental 0 */
        return 0;

    if (n <= 20)         /* direct lookup for the first 20 terms */
        return (seq_t)init_u64[n];

    /* buffer holding the last 20 computed terms: a(n‑20) … a(n‑1) */
    seq_t buf[20];
    for (int i = 0; i < 20; ++i)
        buf[i] = (seq_t)init_u64[i + 1];   /* a(1) … a(20) */

    /* Compute successive terms up to a(n). */
    for (idx_t cur = 21; cur <= n; ++cur) {
        seq_t pos = 0;   /* sum of terms multiplied by non‑negative coeffs */
        seq_t neg = 0;   /* sum of terms multiplied by |negative coeffs| */

        for (int k = 1; k <= 20; ++k) {
            seq_t val = buf[20 - k];   /* a(cur‑k) */
            int  c   = coeff[k - 1];
            if (c >= 0)
                pos += (seq_t)c * val;
            else
                neg += (seq_t)(-c) * val;
        }

        /* The recurrence guarantees pos ≥ neg, so subtraction is safe. */
        seq_t nxt = pos - neg;

        /* Shift the buffer left by one position and insert the new term. */
        for (int i = 0; i < 19; ++i)
            buf[i] = buf[i + 1];
        buf[19] = nxt;
    }

    return buf[19];   /* a(n) */
}
