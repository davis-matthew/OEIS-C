/*
 * A290950  Positive integers whose digits are in increasing order and whose
 *          successive digits differ by exactly 2.
 * https://oeis.org/A290950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A290950: Positive integers whose digits are in increasing order
   and successive digits differ by exactly 2. The sequence is finite
   (lengths 1–5, 25 numbers total). This function computes a(n) by
   combinatorial enumeration of the digit length and starting digit. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n = 0 */
    idx_t L = 0;                          /* length of the number */

    /* Find the length L such that C(L‑1) < n ≤ C(L),  C(L) = L*(10‑L) */
    for (idx_t l = 1; l <= 5; ++l) {
        idx_t cumulative = l * (10 - l);
        if (n <= cumulative) { L = l; break; }
    }

    if (L == 0) return 0;                 /* n exceeds the defined terms */

    idx_t prev_cumulative = (L - 1) * (10 - (L - 1));
    idx_t offset = n - prev_cumulative - 1;   /* 0‑based index within L‑digit terms */
    seq_t start_digit = (seq_t)(1 + offset);

    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i) {
        seq_t digit = start_digit + (seq_t)(2 * i);
        value = value * 10 + digit;
    }

    return value;
}
