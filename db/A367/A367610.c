/*
 * A367610  Comma transform of A367362.
 * https://oeis.org/A367610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = first_digit(n) * 10 +
 *            last_digit_after_zero_replacement(n) where in the decimal
 *            representation of n every zero is replaced by the preceding
 *            non‑zero digit (the digit produced just before the zero)
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the comma transform of A367362 for any n >= 1.  */
seq_t a(idx_t n)
{
    /* Handle n==0 separately (not part of the OEIS offset). */
    if (n == 0)
        return (seq_t)0;

    /* Extract the decimal digits of n into an array (least significant first). */
    unsigned int digits[20];             /* 2^64‑1 has at most 20 decimal digits */
    int          cnt = 0;
    idx_t        tmp = n;
    while (tmp > 0) {
        digits[cnt++] = (unsigned int)(tmp % 10);
        tmp /= 10;
    }

    /* The most significant digit is the first digit of the transformed number. */
    seq_t first_digit = (seq_t)digits[cnt - 1];

    /* Scan the digits from most significant to least, applying the zero‑replacement
       rule and keeping track of the last digit produced. */
    seq_t last_nonzero = first_digit;
    seq_t last_output = first_digit;
    for (int i = cnt - 2; i >= 0; --i) {
        unsigned int d = digits[i];
        seq_t out;
        if (d == 0)
            out = last_nonzero;     /* replace zero with preceding digit */
        else {
            out = d;
            last_nonzero = out;     /* new non‑zero digit becomes the precedent */
        }
        last_output = out;
    }

    /* Combine first and last digits to form the sequence value. */
    return first_digit * (seq_t)10 + last_output;
}
