/*
 * A379268  Numbers with only digits "1" and two digits "0".
 * https://oeis.org/A379268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007088(A379267(n))
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A sequence of decimal numbers whose digits are all '1' except exactly two '0's. */

/* Compute a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    /* Find minimal length L such that C(L,3) >= n.
       C(L,3) counts all numbers of length up to L. */
    idx_t L = 3;
    for (;;) {
        idx_t total = L * (L - 1) * (L - 2) / 6;   /* C(L,3) */
        if (total >= n)
            break;
        ++L;
    }

    /* Combinations before length L: C(L-1,3) */
    idx_t prev = (L - 1) * (L - 2) * (L - 3) / 6;
    idx_t t = n - prev - 1;          /* 0‑based rank within length L */

    idx_t k = L - 1;                  /* usable positions are 1 .. k */
    idx_t a = 0, b = 0;

    /* Find the zero positions a < b from the lexicographic rank t.
       For each first zero a, there are (k - a) possible second zeros. */
    for (idx_t pos = 1; pos <= k - 1; ++pos) {
        idx_t count = k - pos;
        if (t < count) {
            a = pos;
            b = pos + 1 + t;
            break;
        }
        t -= count;
    }

    /* Build the decimal value digit by digit. */
    seq_t val = 0;
    for (idx_t pos = 0; pos < L; ++pos) {
        seq_t digit = ((pos == a) || (pos == b)) ? 0 : 1;
        val = val * 10 + digit;
    }
    return val;
}
