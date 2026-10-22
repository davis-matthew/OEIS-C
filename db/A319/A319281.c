/*
 * A319281  Numbers of the form 16^i*(16*j + 1).
 * https://oeis.org/A319281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer of the form 16^i * (16*j + 1)
 * Range:     u8 n <= 17, u16 n <= 4369, u32 n <= 9807, u64 n <= 9807,
 *            u128 n <= 9807, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the sequence is undefined; return 0 for safety. */
    if (n == 0)
        return 0;

    idx_t found = 0;
    seq_t x = 1;

    for (;;) {
        /* Count the exponent of 2 in x (the 2‑adic valuation). */
        seq_t tmp = x;
        unsigned trailing = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++trailing;
        }

        /* x satisfies the requirement iff
           1) trailing % 4 == 0   (valuation divisible by 4)
           2) odd part modulo 16 == 1   (tmp & 0xF == 1) */
        if ((trailing & 3) == 0 && (tmp & ((seq_t)15)) == (seq_t)1) {
            ++found;
            if (found == n)
                return x;
        }
        ++x;   /* next candidate */
    }
}
