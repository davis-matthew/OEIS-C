/*
 * A292372  A binary encoding of 2-digits in base-4 representation of n.
 * https://oeis.org/A292372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9998, u16 n <= 9998, u32 n <= 9998, u64 n <= 9998,
 *            u128 n <= 9998, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A292372 – Binary encoding of 2‑digits in base‑4 representation of n.
   For each base‑4 digit of n, replace it by 1 if the digit equals 2 and by
   0 otherwise.  Interpret the resulting binary string as a binary number.
   The function returns this number as a seq_t. */

seq_t a(idx_t n)
{
    seq_t res = 0;
    int digits[64];          /* sufficient for 64‑bit indices */
    int len = 0;
    idx_t t = n;

    /* Extract base‑4 digits (least significant first) */
    while (t > 0) {
        digits[len++] = t % 4;
        t /= 4;
    }

    /* Build the binary result from most significant digit to least */
    for (int i = len - 1; i >= 0; --i) {
        res = (res << 1) | (digits[i] == 2 ? 1 : 0);
    }

    return res;
}
