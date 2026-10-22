/*
 * A343823  Numbers k > 10 such that every permutation of the digits of k is
 *          congruent to 3 (mod 4).
 * https://oeis.org/A343823
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k > 10 whose decimal digits are only 1,5,9.
   All permutations of such a number are ≡ 3 (mod 4).  The sequence
   is listed in increasing order with offset 11 (a(11) = 11). */

seq_t a(idx_t n)
{
    /* Handle indices below the offset – behavior is unspecified. */
    if (n < 11) return 0;

    /* m counts the position within the sequence starting from 1. */
    idx_t m = n - 10;
    idx_t len = 2;          /* minimal length is 2 (numbers >10) */
    idx_t power = 9;        /* 3^2 */

    /* Find the length of the m‑th number. */
    while (m > power) {
        m -= power;
        len++;
        power *= 3;        /* next power 3^len */
    }

    /* 0‑based index inside the group of numbers with this length. */
    idx_t idx0 = m - 1;

    /* Pre‑compute powers of 3 up to len. */
    const int MAX_LEN = 64;
    idx_t pow3[MAX_LEN];
    pow3[0] = 1;
    for (int i = 1; i <= (int)len; ++i)
        pow3[i] = pow3[i - 1] * 3;

    /* Build the decimal number from most significant digit to least. */
    seq_t res = 0;
    for (int i = 0; i < (int)len; ++i) {
        idx_t pow = pow3[(int)len - 1 - i];
        idx_t d   = idx0 / pow;          /* digit in base‑3 (0..2) */
        idx0 -= d * pow;                 /* remaining part */

        int digit = (d == 0 ? 1 : (d == 1 ? 5 : 9));
        res = res * 10 + digit;
    }

    return res;
}
