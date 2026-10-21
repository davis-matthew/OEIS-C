/*
 * A266704  Coefficient of x^2 in minimal polynomial of the continued fraction
 *          [1^n,2/3,1,1,1,...], where 1^n means n ones.
 * https://oeis.org/A266704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 20, i64 n <= 44, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A266704:  a(n) = 2*a(n‑1) + 2*a(n‑2) – a(n‑3)  for n ≥ 6
   Initial terms: a(0) = –3, a(1) = –19, a(2) = 17,
   a(3) = –75, a(4) = –165, a(5) = –463. */
seq_t a(idx_t n)
{
    /* Pre‑computed initial values */
    const seq_t init[6] = { -3, -19, 17, -75, -165, -463 };

    if (n < 6) return init[n];

    /* sliding window: x = a(i‑3), y = a(i‑2), z = a(i‑1) */
    seq_t x = init[3];  /* a(3) */
    seq_t y = init[4];  /* a(4) */
    seq_t z = init[5];  /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t new_val = 2 * z + 2 * y - x;  /* recurrence */
        x = y;
        y = z;
        z = new_val;
    }
    return z;
}
