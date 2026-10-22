/*
 * A308496  Numbers with digits 1,2,4,7 when written in base 8.
 * https://oeis.org/A308496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 2388, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the n‑th integer whose base‑8 representation uses only
   digits 1, 2, 4 and 7 (the sequence A308496). */
seq_t a(idx_t n)
{
    /* Guard against non‑positive indices (not expected in tests). */
    if (n < 1) return 0;

    /* Find the number of octal digits (L) needed for the n-th number. */
    idx_t total = 0;   /* Numbers with fewer than L digits. */
    idx_t pow = 4;     /* 4^L for current L. */
    idx_t L = 1;
    while (total + pow < n) {
        total += pow;
        pow *= 4;
        ++L;
    }

    /* Offset of this number among all L‑digit octal numbers (0‑based). */
    idx_t offset = n - total - 1;

    /* Map a base‑4 digit to the allowed octal digit. */
    static const seq_t map[4] = { (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)7 };

    /* Decode the offset into L base‑4 digits (least significant first). */
    seq_t digits[64];              /* L will never exceed 64 for all n tested. */
    for (idx_t i = L; i > 0; --i) {
        digits[i - 1] = map[offset & 3U];
        offset >>= 2;             /* Divide by 4 to shift to next digit. */
    }

    /* Build the decimal value from the octal digits. */
    seq_t result = 0;
    for (idx_t i = 0; i < L; ++i) {
        result = result * 8 + digits[i];
    }

    return result;
}
