/*
 * A287438  Start with 0 and repeatedly substitute 0->012, 1->120, 2->210.
 * https://oeis.org/A287438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A287438: fixed point 0->012, 1->120, 2->210
   by traversing the base‑3 representation of (n‑1). */
seq_t a(idx_t n)
{
    /* mapping table: map[d][i] is the i‑th symbol produced from d */
    const int map[3][3] = {{0,1,2},
                           {1,2,0},
                           {2,1,0}};
    idx_t p = n - 1;                    /* zero–based index */
    int digits[64];                     /* base‑3 digits (LSB first) */
    int len = 0;
    do
    {
        digits[len++] = (int)(p % 3);
        p /= 3;
    } while (p);
    seq_t sym = 0;                      /* start with the initial 0 */
    for (int i = len - 1; i >= 0; --i)
    {
        sym = (seq_t)map[(int)sym][digits[i]];
    }
    return sym;
}
