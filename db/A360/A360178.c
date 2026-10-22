/*
 * A360178  Decimal expansion of the molar Planck constant (N_A*h) according
 *          to the 2019 SI system in units J / (Hz*mol).
 * https://oeis.org/A360178
 *
 * Offset:    -9    (a(-9) is the first term)
 * Keywords:  cons, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 7, u128 n <= 7, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360178: digits of the decimal expansion of the molar Planck constant
   (N_A*h = 0.00000000039903127128934314).  The sequence starts at the
   first non‑zero digit after the nine leading zeros, so a(-9) is that digit.
   a(n) is the (n+9)th digit of the integer 399031271289343140;
   a(n)=0 for n < -9 or n > 8. */

seq_t a(idx_t n)
{
    /* Indices outside the finite non‑zero part are zero. */
    if (n < -9 || n > 8)
        return 0;

    /* Position in the 18‑digit integer 399031271289343140. */
    idx_t idx = n + 9;   /* idx ∈ [0,17] */

    /* Constant integer value of the numerator (fits in 64‑bit). */
    static const seq_t num = (seq_t)399031271289343140ULL;

    /* Powers of 10: 10^0 … 10^17. */
    static const seq_t pow10[18] = {
        (seq_t)1ULL,           (seq_t)10ULL,          (seq_t)100ULL,
        (seq_t)1000ULL,        (seq_t)10000ULL,       (seq_t)100000ULL,
        (seq_t)1000000ULL,     (seq_t)10000000ULL,    (seq_t)100000000ULL,
        (seq_t)1000000000ULL,  (seq_t)10000000000ULL, (seq_t)100000000000ULL,
        (seq_t)1000000000000ULL, (seq_t)10000000000000ULL,
        (seq_t)100000000000000ULL, (seq_t)1000000000000000ULL,
        (seq_t)10000000000000000ULL, (seq_t)100000000000000000ULL
    };

    /* Extract the digit: (num / 10^(17-idx)) % 10. */
    seq_t digit = (num / pow10[17 - idx]) % 10;
    return digit;
}
