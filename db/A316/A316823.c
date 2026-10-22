/*
 * A316823  Balanced nonary enumeration (or balanced nonary representation) of
 *          integers; write n in nonary (base 9) and then replace 5's with
 *          (-4)'s, 6's with (-3)'s, 7's with (-2)'s, and 8's with (-1)'s.
 * https://oeis.org/A316823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 161, i16 n <= 6560, i32 n <= 6560, i64 n <= 6560,
 *            i128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced nonary representation.
   a(n) is obtained by writing n in base 9 and, for each digit d:
     if 0 ≤ d ≤ 4, use d;
     if 5 ≤ d ≤ 8, use d – 9  (i.e. 5→–4, 6→–3, 7→–2, 8→–1).
   Then a(n) = Σ (balanced_digit_i) · 9^i. */
seq_t a(idx_t n)
{
    seq_t val = 0;    /* accumulated result */
    seq_t pow9 = 1;   /* current power of 9 (9^i) */
    idx_t x = n;      /* work on a copy of n */

    while (x > 0) {
        idx_t d = x % 9;                 /* next base‑9 digit (0..8) */
        int nd = (int)d;                 /* promote to signed int first */
        seq_t digit = nd < 5 ? (seq_t)nd : (seq_t)(nd - 9); /* balanced value */
        val += digit * pow9;
        pow9 *= 9;
        x /= 9;
    }

    return val;
}
